#include "Scripting.hpp"
namespace scr {
	const char* jslocation = "data\\game\\js\\";
	duk_context* _context;
	void* my_udata = (void*)0x0;
	bool _init = false;
	namespace scripts {
		//https://github.com/svaarala/duktape/blob/master/extras/console/duk_console.c#L23
		static duk_ret_t duk__console_log_helper(duk_context* ctx, const char* error_name) {
			FILE* output = stdout;
			duk_idx_t n = duk_get_top(ctx);
			duk_idx_t i;

			duk_get_global_string(ctx, "console");
			duk_get_prop_string(ctx, -1, "format");

			for (i = 0; i < n; i++) {
				if (duk_check_type_mask(ctx, i, DUK_TYPE_MASK_OBJECT)) {
					/* Slow path formatting. */
					duk_dup(ctx, -1);  /* console.format */
					duk_dup(ctx, i);
					duk_call(ctx, 1);
					duk_replace(ctx, i);  /* arg[i] = console.format(arg[i]); */
				}
			}

			duk_pop_2(ctx);

			duk_push_string(ctx, " ");
			duk_insert(ctx, 0);
			duk_join(ctx, n);

			if (error_name) {
				duk_push_error_object(ctx, DUK_ERR_ERROR, "%s", duk_require_string(ctx, -1));
				duk_push_string(ctx, "name");
				duk_push_string(ctx, error_name);
				duk_def_prop(ctx, -3, DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE);  /* to get e.g. 'Trace: 1 2 3' */
				duk_get_prop_string(ctx, -1, "stack");
			}

			fprintf(output, "%s\n", duk_to_string(ctx, -1));
			fflush(output);
			return 0;
		}
		static duk_ret_t duk__log(duk_context* ctx) {
			return duk__console_log_helper(ctx, NULL);
		}
		static duk_ret_t duk__error(duk_context* ctx) {
			return duk__console_log_helper(ctx, "Error");
		}
		//filesystem functions
		static duk_ret_t readFileB(duk_context* ctx) {
			std::ifstream inp(duk_require_string(ctx, 0), std::ios::binary);
			std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inp), {});
		}

		static duk_ret_t require(duk_context* ctx) {
		}
	}
	static void fatal_handler(void* udata, const char* msg) {
		//stolen from https://wiki.duktape.org/howtofatalerrors#example-fatal-error-handler
		(void)udata;
		fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
		fflush(stderr);
		abort();
	}
	int initialize() {
		if (_init) {
			printf("Javascript engine already initialized! You shouldn't be seeing this\n");
			return 1;
		}
		_context = duk_create_heap(NULL,NULL,NULL,my_udata,fatal_handler);
		const duk_function_list_entry funcs[] = {
			{"log", scripts::duk__log, DUK_VARARGS},
			{"error", scripts::duk__error, DUK_VARARGS},
			{ NULL, NULL, 0 }
		};
		duk_push_global_object(_context);
		duk_push_object(_context);
		duk_put_function_list(_context, -1, funcs);
		duk_eval_string(_context,
			"(function (E) {"
			"return function format(v){"
			"try{"
			"return E('jx',v,null,4);"
			"}catch(e){"
			"console.log(\"failed to convert to json\");"
			"return String(v);"  /* String() allows symbols, ToString() internal algorithm doesn't. */
			"}"
			"};"
			"})(Duktape.enc)");
		duk_put_prop_string(_context, -1, "format");
		duk_put_prop_string(_context, -2, "console");
		duk_pop(_context);
		return 0;
	}
	void evalFile(const char* filename) {
		std::string code,line;
		std::string loca(jslocation);
		std::string fn(filename);
		printf("%s %s", loca.c_str(), fn.c_str());
		loca += fn;
		std::ifstream file(loca);
		if (file.is_open()) {
			while (getline(file, line)) {
				code += line;
				code += "\n";
			}
			file.close();
			duk_push_string(_context, code.c_str());
			printf("%s", code.c_str());
			if (duk_peval(scr::_context) != 0) {
				printf("Script \"%s\" error: %s\n", loca.c_str() , duk_safe_to_string(scr::_context, -1));
			}
		}
		else {
			printf("File \"%s\" couldn't open!", loca.c_str());
		}
	}
	void quit() {
		if (!_init) return;
		duk_destroy_heap(_context);
	}
}