include <constants.scad>
use <mounting_holes.scad>

L_BATT = 68;
W_BATT = 23;
H_BATT = 15;
D_BATT = 19;

module battery_mounting_holes(d=2.8){
  mounting_holes([25,23/2+5+W_WALL],d);
}


module battery_cover_cutout_2d(d=2.8) {
  square([68,23], center=true);
  battery_mounting_holes(d);
}

module battery_cover() {
  module profile(){
    hull(){
      translate([W_BATT/2-D_BATT/2,H_BATT-D_BATT/2])circle(d=D_BATT);
      translate([-W_BATT/2,-.1])square([W_BATT,W_WALL+.1]);
    }
  }

  rotate([90,0,90])difference(){
    linear_extrude(L_BATT+2*W_WALL,center=true)difference(){
      offset(W_WALL) profile();
      translate([-100,-100])square([200,100]);
    }
    linear_extrude(L_BATT, center=true) profile();
  }
  linear_extrude(W_WALL){
    difference(){
      hull()battery_mounting_holes(8);
      battery_cover_cutout_2d(3.2);
    }
  }
}

$fn=72;
battery_cover();
