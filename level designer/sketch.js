var cell_h = 14;
var cell_w = 7;
var map_h = 31;
var map_w = 100;

var cells = [
  new Cell("air", " ", ".", [0,0,0]),
  new Cell("wall", " ", "W", [255, 255, 255], [0,0,0]),
  new Cell("start", " ", "S", [0, 255, 0]),
  new Cell("end", " ", "E", [0, 0, 255]),
  new Cell("lava", " ", "L", [255, 0, 0]),
  new Cell("portalA", "O", "A", [0, 0, 0], [255, 0, 255]),
  new Cell("portalB", "O", "B", [0, 0, 0], [255, 255, 0]),
  new Cell("ladder", "#", "#", [0, 0, 0], [255, 255, 255]),
  new Cell("zonaportal", "^", "Z", [0, 0, 0], [0, 255, 0]),
  new Cell("desapear_wall", " ", "D", [124, 124, 124], [0, 255, 0])
];


var index = 1;
var myP;
var lines = true;

function setup() {
  myMap = [];
  for(let i = 0; i < map_h; i++){
    let tmp = [];
    for(let j = 0; j < map_w; j++) tmp.push(cells[0]);
    myMap.push(tmp);
  }

  createCanvas(800, 600);
  background(50);
  noStroke();

  let bg_color;
  let fg_color;
  let char;
  textSize(cell_w*4);
  for(let i = 0; i < cells.length; i++){
    bg_color = cells[i].bck_color;
    char = cells[i].draw_char;
    fg_color = cells[i].fg_color;
    fill(bg_color);
    rect(i*(cell_w*5) + 30, map_h*cell_h + 60, 4*cell_w, 4*cell_h);
    if(char != " "){
      fill(fg_color);
      text(char, i*(cell_w*5) + 33, map_h*cell_h + 100);
    }
  }
  textSize(cell_w);
}

function draw() {
  let bg_color;
  let fg_color;
  let char;
  for(let i = 0; i < map_h; i++){
    for(let j = 0; j < map_w; j++){
      bg_color = myMap[i][j].bck_color;
      char = myMap[i][j].draw_char;
      fg_color = myMap[i][j].fg_color;
      fill(bg_color);
      rect(j*cell_w + 30, i*cell_h + 30, cell_w, cell_h);
      if(char != " "){
        fill(fg_color);
        text(char, j*cell_w + 30, (i + 0.66)*cell_h + 30);
      }
    }
  }
  if(mouseX > 30 && mouseX < 30 + cell_w*map_w && mouseY > 30 && mouseY < 30 + cell_h*map_h){
    let i = floor((mouseY - 30)/cell_h);
    let j = floor((mouseX - 30)/cell_w);
    bg_color = cells[index].bck_color;
    char = cells[index].draw_char;
    fg_color = cells[index].fg_color;
    fill(bg_color);
    rect(j*cell_w + 30, i*cell_h + 30, cell_w, cell_h);
    if(char != " "){
      fill(fg_color);
      text(char, j*cell_w + 30, (i + 0.66)*cell_h + 30);
    }
  }
  if(lines){
    strokeWeight(1);
    stroke(64);
    for(let i = 1; i < map_w; i++) line(30 + i*cell_w,30, 30 + i*cell_w,cell_h*map_h + 30);
    for(let i = 1; i < map_h; i++) line(30, 30 + i*cell_h, 30 + cell_w*map_w, 30 + i*cell_h);
    noStroke();
  }
}

function mousePressed() {
  changeMap();
}
function mouseDragged(){
  changeMap();
}
function changeMap(){
  if(mouseX > 30 && mouseX < 30 + cell_w*map_w && mouseY > 30 && mouseY < 30 + cell_h*map_h){
    let i = floor((mouseY - 30)/cell_h);
    let j = floor((mouseX - 30)/cell_w);
    myMap[i][j] = cells[index];
  }

  for(let i = 0; i < cells.length; i++){
    if(mouseX > i*cell_w*5 + 30 && mouseX < i*cell_w*5 + cell_w*4 + 30 &&
       mouseY > map_h*cell_h + 60 && mouseY < map_h*cell_h + 60 + 4*cell_h){
         index = i;
    }
  }
}
function download(filename, text) {
  var element = document.createElement('a');
  element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
  element.setAttribute('download', filename);
  element.style.display = 'none';
  document.body.appendChild(element);
  element.click();
  document.body.removeChild(element);
}

function keyPressed() {
  if(keyCode === 82){ // R
    for(let i = 0; i < map_h; i++){
      for(let j = 0; j < map_w; j++) myMap[i][j] = cells[0];
    }
  }
  else if(keyCode >= 49 && keyCode <= 48+cells.length) index = keyCode - 49;
  else if (keyCode == 76) lines = !lines; // L
  else if (keyCode == 67 || keyCode == 68){ //C or D
    p = "";
    for(let i = 0; i < map_h; i++){
      for(let j = 0; j < map_w; j++){
          p = p.concat(myMap[i][j].read_char);
      }
      p = p.concat("\n")
    }
    if(keyCode == 67){
      myP = createElement('textarea', p);
      myP.id("te");
      let cp = document.getElementById('te');
      cp.select();
      document.execCommand("copy");
      myP.remove();
    }else{
      download("myMap.txt", p)
    }
  }
}
