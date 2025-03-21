include <constants.scad>
use <bartscad/poly.scad>
use <pcb.scad>
use <keyboard.scad>
use <parts.scad>
use <display.scad>
use <battery_cover.scad>
use <captive_nut.scad>

Z_OFFSET_TOP = 1;
Z_PCB=4+W_WALL;
H_CASE = h_pcb()+Z_PCB;

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

PCB_POSITION = [0.2,45,4+W_WALL];

module pcb_anchor() {
  translate(PCB_POSITION)rotate([0,0,180])children();
}

function keyboard_anchor(x=55) = [x,0,H_CASE-h_keyboard()];

module keyboard_anchor() {
  translate(keyboard_anchor()) children();
}

function keyboard_anchor_right() = keyboard_anchor(15);

module keyboard_anchor_right() {
  translate(keyboard_anchor_right()) children();
}

module support_strip_gaps(extend=false) {
    for(y = [Y_MIN+3/2+W_WALL, Y_MAX-3/2-W_WALL]){
      translate([-40,y,H_CASE/2]){
        rotate([0,90,0])linear_extrude(300,center=extend)square([15.2,2.2],center=true);
        for(x=[8,20]){
          translate([x,0,0])rotate([90,0,0])cylinder(d=2.8,h=10,center=true);
        }
      }
    }
}

module case_gaps() {
    // space for PCB
    pcb_anchor() difference(){
      union(){
        pcb_hole(.2, top_open=true);
        translate([10,0,0])scale([-1,1,1])linear_extrude(H_CASE+1)offset(.2)square([30,90]);
      }
      pcb_feet(1);
    }
    pcb_anchor() at_pcb_screw_positions() insert_m3();

    // captive nuts and screw holes for strap mounts
    for (y=[55,-55]) {
      translate([X_MIN+10,y,H_CASE/2])rotate([0,-90,0])captive_nut_screw_hole_m5($tolerance=0.1);
    }
    
    // space for keyboard
    keyboard_anchor()keyboard(anchor="left",open_top=true);
    // diagonal cutoff
    keyboard_anchor()translate([keyboard_overlap_top()+W_WALL,0,0])keyboard_edge_top_left()rotate([0,80,0]) linear_extrude(15) square(200,center=true);

    // space for cable
    difference(){
      pcb_anchor()scale([-1,1,1]) linear_extrude(H_CASE) offset(0.2)square([50,90]);
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
    translate([X_MAX,0])support_strip_gaps();
}

module case_bottom() {
  difference(){
    linear_extrude(H_CASE+H_BORDER) case_footprint();
    case_gaps();
  }

  module ledge_profile(){
    polygon([
      [X_MIN+W_WALL-0.01,H_CASE],
      [X_MIN+W_WALL+2,H_CASE],
      [X_MIN+W_WALL+2,H_CASE-1],
      [X_MIN+W_WALL-0.01,H_CASE-3],
    ]);

  }

  translate([0,PCB_POSITION.y+1,0])rotate([90,0,0])linear_extrude(90-38.9425-21.1/2)ledge_profile();
  translate([0,PCB_POSITION.y-90+38.9425-21.1/2-1,0])rotate([90,0,0])linear_extrude(38.9425-21.1/2)ledge_profile();
}


module switch_mount_2d(omit_handle=false){
  translate([15,0]) circle(d=3.2);
  translate([-15,0]) circle(d=3.2);
  if(!omit_handle) square([13.5,6.5], center=true);
}

module case_top_2d(positive=true){
  difference() {
    offset(positive ? -W_WALL-0.1: -W_WALL)case_footprint();
    offset(positive? 0.1:0)translate([keyboard_anchor().x+keyboard_overlap_top()+keyboard_top_max_x(),Y_MIN-1])square([10,Y_MAX-Y_MIN+2]);
    if(positive){
      for(pos=SCREW_POSITIONS) {
        translate(pos)circle(d=3.2);
      }
      translate([PCB_POSITION.x,PCB_POSITION.y]) rotate([0,0,180])translate([23.86+11,37+26]){
        offset(3)square([22,52], center=true);
        translate([28/2+5,22])circle(d=2.8);
        translate([-28/2-5,22])circle(d=2.8);
        translate([28/2+5,-22])circle(d=2.8);
        translate([-28/2-5,-22])circle(d=2.8);
      }
      translate([-50,32]) switch_mount_2d();
      translate([PCB_POSITION.x+16,PCB_POSITION.y-11.5,0]){
        battery_cover_cutout_2d();
      }
      translate([20,-23])rotate([0,0,30])display_cutout_2d();
    }
  }
}

module switch_case() {
  SWITCH_POS=[-48,50];
  module outline(){
    offset(3.3)hull(){
      translate([-50,32])switch_mount_2d();
      translate(SWITCH_POS)offset(2)switch_mount_2d();
      translate(SCREW_POSITIONS[1])circle(d=3.4);
    }
  }
  difference(){
    linear_extrude(15)outline();
    translate([0,0,-W_WALL])linear_extrude(15) offset(-3) outline();
    linear_extrude(16){
      translate([-50,32])switch_mount_2d(true);
      translate(SWITCH_POS)switch_mount_2d();
      translate(SCREW_POSITIONS[1])circle(d=3.4);
    }
  }
}


module case_top(positive=true) {
  translate([0,0,H_CASE]) {
    linear_extrude(W_WALL) case_top_2d(positive);
  }
}

$fn=120;
case_bottom();


translate([100,0,0]){
  case_bottom_right();
  translate([0,0,10])case_top_right();
}

#color("#dd2222")case_top();
translate([0,0,H_CASE+3])switch_case();
translate([16,33.5,H_CASE+3])battery_cover();
