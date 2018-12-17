function draw() {
  var canvas = document.getElementById("canvas11");
  if(canvas.getContext) {
    var ctx = canvas.getContext("2d");
    ctx.fillStyle = "rgb(200,0,0)";
    ctx.fillRect(100, 100, 255, 250);

    ctx.beginPath();
    ctx.moveTo(175,250);
    ctx.lineTo(100,275);
    ctx.lineTo(100,235);
    ctx.lineTo(75,250);
    ctx.stroke();

    ctx.fillStyle = "rgba(0, 0, 200, 0.25)";
    ctx.fillRect(130, 130, 255, 250);
  }
}
