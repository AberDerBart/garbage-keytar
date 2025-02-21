include <BOSL/constants.scad>
use <BOSL/paths.scad>

use <bartscad/poly.scad>

use <../parts.scad>
include <../constants.scad>

TOLERANCE=0.1;

W = 50;
H = 30;
L = 160;

L_STRIP = 116+2*TOLERANCE;
W_STRIP = 20.52+2*TOLERANCE;
H_STRIP = 0.6+TOLERANCE;

L_STRIP_ACTIVE = 100;
W_STRIP_ACTIVE = 7.11;

SCREW_HOLES = [
  [0,7],
  [W/2-5,H-7],
  [-W/2+5,H-7],
];

module for_neck_screw_positions() {
  for(p = SCREW_HOLES){
    translate(p) children();
  }
}

module for_top_screw_holes(){
  multmatrix([
    [1,0,0],
    [0,0,1],
    [0,1,0],
  ])for(y=[5,L/2,L-5])for(x = [13,-13]){
    translate([x,y,H-H_BORDER-TOLERANCE])children();
  }
}

module neck_cross(){
  difference(){
    poly([
      fillet([-W/2, 0], W/2),
      fillet([W/2, 0], W/2),
      fillet([W/2, H],5),
      fillet([-W/2, H],5),
    ]);
    for_neck_screw_positions() {
      circle(d=4.4);
    }
  }
}

module neck_top_profile(){
    translate([0, H-H_BORDER/2])square([34, H_BORDER], center=true);
}

module neck_top(){
  difference(){
    linear_extrude(L) neck_top_profile();
    for_top_screw_holes(){
      translate([0,0,H_BORDER-1.4+TOLERANCE])cylinder(r1=1.7, r2=3.2,h=1.5);
      cylinder(r=1.7,h=10, center=true);
    }
    multmatrix([
      [1,0,0],
      [0,0,1],
      [0,1,0],
    ]) #translate([0,L/2,H-H_BORDER]){
      hull(){
        translate([0,L_STRIP_ACTIVE/2,-0.1])cylinder(r1=W_STRIP_ACTIVE/2+2, r2=W_STRIP_ACTIVE/2+2+H_BORDER+0.2,h=H_BORDER+0.2);
        translate([0,-L_STRIP_ACTIVE/2,-0.1])cylinder(r1=W_STRIP_ACTIVE/2+2, r2=W_STRIP_ACTIVE/2+2+H_BORDER+0.2,h=H_BORDER+0.2);
      }
      let ($fn=6) translate([0,0,-0.1]) rotate([0,0,90])cylinder(r1=8,r2=8+H_BORDER+0.2,h=H_BORDER+0.2);
    }
  }
}

module neck_bottom(){
  difference(){
    neck_cross();
    offset(TOLERANCE) neck_top_profile();
  }
}

module neck_end(){
  intersection(){
    multmatrix([
      [1,0,0],
      [0,0,1],
      [0,1,0],
    ]) 
      scale([1,1.7,1])
      rotate_extrude(angle=180)
      intersection()
    {
      neck_cross();
      square(100);
    }

    cube(60, center=true);
  }
}

module pitchstrip_cutout(){
  translate([-W_STRIP/2,H-H_BORDER-H_STRIP,L/2-L_STRIP/2])cube([W_STRIP, H_STRIP, L_STRIP]);
}

module neck_cable_channel(additional_path_points=[]){
  module _2d(){
    square([15,7], center=true);
  }
  extrude_2d_shapes_along_3dpath([
    [0,H,L/2-54],
    [0,18,max(L/2-70,0)],
    [0,18,-1],
    each additional_path_points
  ]) _2d();
}

module neck(){
  difference(){
    linear_extrude(L)neck_bottom();
    pitchstrip_cutout();
    neck_cable_channel();
    for_top_screw_holes(){
      insert_m3();
    }
  }
}

$fn=160;
neck();

translate([0,10,0])neck_top();

!linear_extrude(40) difference(){
  neck_cross();
  translate([0,18]) square([15,7], center=true);
}
