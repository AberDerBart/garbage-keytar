include <constants.scad>
use <case.scad>
use <keyboard.scad>
use <parts.scad>
use <bartscad/poly.scad>

use <head/pitch_wheel.scad>
use <head/neck.scad>
use <bartscad/stack.scad>

include <keyboard.scad>

SCREW_POSITIONS_RIGHT = [
  [X_MAX_RIGHT-15, Y_MIN+15],
  [X_MAX_RIGHT-10, 0],
  [X_MAX_RIGHT-15, Y_MAX-15],
  [keyboard_anchor_right().x-keyboard_top_min_x()+keyboard_overlap_top()+W_WALL-10,Y_MIN+10],
  [keyboard_anchor_right().x+5+W_WALL,0],
  [keyboard_anchor_right().x-keyboard_top_min_x()+keyboard_overlap_top()+W_WALL-10,Y_MAX-10],
];

module case_footprint_right(){
  poly([
    [X_MIN_RIGHT,Y_MIN],
    fillet([X_MAX_RIGHT,Y_MIN],R_CORNER),
    fillet([X_MAX_RIGHT,Y_MAX],R_CORNER),
    [X_MIN_RIGHT,Y_MAX],
  ]);
}

module case_bottom_right() {
  difference(){
    linear_extrude(H_CASE+H_BORDER) case_footprint_right();
    case_gaps_right();
  }
}

module case_top_right(positive=true) {
  translate([0,0,H_CASE]) linear_extrude(W_WALL) difference(){
      offset(positive ? -W_WALL-0.1: -W_WALL)case_footprint_right();
    
    offset(positive? 0.1:0)translate([keyboard_anchor_right().x-keyboard_overlap_top()+keyboard_top_min_x(),Y_MIN-1])scale([-1,1])square([10,Y_MAX-Y_MIN+2]);

    if(positive)
      for(pos=SCREW_POSITIONS_RIGHT)
    {
      translate(pos)circle(d=3.2);
    }
  }
}

module neck_anchor(){
  translate([X_MAX_RIGHT, -30, 0])multmatrix([[0,0,1],[1,0,0],[0,1,0]]) children();
}

module case_gaps_right() {
  // space for keyboard
  keyboard_anchor_right()keyboard(anchor="right",open_top=true);
  // diagonal cutoff
  keyboard_anchor_right()translate([-keyboard_overlap_top()-W_WALL,0,0])keyboard_edge_top_right()rotate([0,-80,0]) linear_extrude(15) square(200,center=true);
  
  // space for top
  case_top_right(false);

  // space for support strips
  translate([X_MIN_RIGHT,0,0])scale([-1,1,1])support_strip_gaps(true);

  // screw insets
  for(pos=SCREW_POSITIONS_RIGHT){
    translate([0,0,H_CASE])translate(pos) insert_m3();
  }

  neck_anchor(){
    for_neck_screw_positions(){
      translate([0,0,-100])cylinder(d=4.4,h=101);
      translate([0,0,-60])let($fn=6) cylinder(d=7.66+0.2,h=25);
    }
    neck_cable_channel([[0,18,-45]]);
    #translate([0,0,-45])linear_extrude(10){
      polygon([
        [-7.5,21.5],
        [-7.5-10,8.6],
        [-7.5-10,5],
        [-7.5,5],
        [-7.5,8.8],
        [-5,14.5],
      ]);
    }
  }

  translate([0,0,8.8-6])linear_extrude(6){
    polygon([
      [10,-30-7.5],
      [10,-30-17.5],
      [8,-W_BOTTOM/2],
      [0,-W_BOTTOM/2],
      [0,-W_BOTTOM/2+2],
      [4,-W_BOTTOM/2+2],
      [8,-30-7.5],
    ]);
  }

  #translate([X_MAX_RIGHT-25,-Y_MAX+15]){
    cylinder(d=5.4,h=100, center=true);
    let($fn=6)translate([0,0,10])cylinder(d=9,h=100);
  }
}

$fn=150;

case_bottom_right();
//case_top_right(true);

//neck_anchor() translate([0,0,10])neck();
