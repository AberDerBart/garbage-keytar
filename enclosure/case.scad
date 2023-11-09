use <bartscad/poly.scad>
use <pcb.scad>
use <keyboard.scad>
use <parts.scad>


W_WALL = 3;
Z_OFFSET_TOP = 1;
Z_PCB=4+W_WALL;
H_CASE = h_pcb()+Z_PCB;
H_BORDER=2;

Y_MIN=-78.5;
Y_MAX=78.5;
X_MIN=-73;
X_MAX=73;
R_CORNER=20;

SCREW_POSITIONS = [
  [X_MIN+15,Y_MIN+15],
  [X_MIN+15,Y_MAX-15],
  [50+W_WALL-5,0],
  [keyboard_anchor().x+keyboard_top_max_x()+keyboard_overlap_top()+W_WALL-10,Y_MIN+10],
  [keyboard_anchor().x+keyboard_top_max_x()+keyboard_overlap_top()+W_WALL-10,Y_MAX-10],
];

module case_footprint(){
  poly([
    fillet([-73,Y_MIN],R_CORNER),
    [73,Y_MIN],
    [73,Y_MAX],
    fillet([-73,Y_MAX],R_CORNER),
  ]);
}

module pcb_anchor() {
  translate([0,39,4+W_WALL])rotate([0,0,180])children();
}

function keyboard_anchor() = [55,0,H_CASE-h_keyboard()];

module keyboard_anchor() {
  translate(keyboard_anchor()) children();
}

module case_gaps() {
    // space for PCB
    pcb_anchor() difference(){
      pcb_hole(0.1, top_open=true);
      pcb_feet();
    }
    pcb_anchor() at_pcb_screw_positions() insert_m3();
    
    // space for keyboard
    keyboard_anchor()keyboard(anchor="left",open_top=true);
    // diagonal cutoff
    keyboard_anchor()translate([keyboard_overlap_top()+W_WALL,0,0])keyboard_edge_top_left()rotate([0,80,0]) linear_extrude(15) square(200,center=true);

    // space for cable
    difference(){
      translate([25+W_WALL,-13,W_WALL]) linear_extrude(H_CASE) square([50,100],center=true);
      translate([50+W_WALL-5,0,0])linear_extrude(H_CASE+1){
        circle(d=10);
        translate([3,0,0])square([5,10],center=true);
      }
    }
    keyboard_anchor()
      translate([0,-8,0])
      rotate([0,90,0])
    {
      linear_extrude(30,center=true)cable_channel_cross();
      translate([0,0,10])rotate([0,45,0])linear_extrude(20)cable_channel_cross();
    }

    // space for top
    case_top(false);

    // space for battery
    translate([-35,42,H_CASE+1])scale([1,1,-1])cube([62,35,11]);
    translate([-35,43,H_CASE+1])scale([1,-1,-1])cube([3,10,11]);

    // screw insets
    for(pos=SCREW_POSITIONS){
      translate([0,0,H_CASE])translate(pos) insert_m3();
    }

    // support strips
    for(y = [Y_MIN+3/2+W_WALL, Y_MAX-3/2-W_WALL]){
      translate([X_MAX-40,y,H_CASE/2]){
        rotate([0,90,0])linear_extrude(300)square([15.2,2.2],center=true);
        for(x=[8,20]){
          translate([x,0,0])rotate([90,0,0])cylinder(d=3.2,h=10,center=true);
        }
      }
    }
}

module case_bottom() {
  difference(){
    linear_extrude(H_CASE+H_BORDER) case_footprint();
    case_gaps();
  }
}

module case_top(with_holes=true) {
  difference(){
    translate([0,0,H_CASE]) linear_extrude(W_WALL) difference(){
      offset(-W_WALL)case_footprint();
      if(with_holes)
        for(pos=SCREW_POSITIONS)
      {
        translate(pos)circle(d=3.2);
      }
    }

    keyboard_anchor()translate([keyboard_overlap_top(),0,0])keyboard_edge_top_left()rotate([0,80.0]) linear_extrude(15) square(200,center=true);

  }
}

$fn=120;
case_bottom();

//#color("#dd2222")case_top();
