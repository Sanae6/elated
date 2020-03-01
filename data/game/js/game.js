const escape = 256;
win.onKeyAction(function (key, action) {
	if (action == "press" && key == escape) win.setShouldClose(true);
});

this.gameloop = function (delta) {
	console.log("loop",delta);
}

win.setTitle("Hello World");