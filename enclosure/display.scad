include <constants.scad>
use <mounting_holes.scad>

DISPLAY_W = 24;
DISPLAY_H = 13.5;
DISPLAY_PCB_W = 29;
DISPLAY_PCB_H = 28;
DISPLAY_D=3;
DISPLAY_ANGLE=40;
OFFSET_DISPLAY_Y=5;

module display_mounting_holes_2d(d=2.8){
  mounting_holes([28/2+W_WALL+5,10], d);
}

module display_cutout_2d(){
  translate([-10,-12])square([20,20]);
  display_mounting_holes_2d();
}

module display_mount() {
  linear_extrude(3) difference() {
    hull() {
      display_mounting_holes_2d(8);
    }
    display_mounting_holes_2d(3.2);
    square([DISPLAY_PCB_W, DISPLAY_PCB_H],center=true);
  }

  module display_anchor() {
    translate([-DISPLAY_PCB_H/2*cos(DISPLAY_ANGLE),0])rotate(DISPLAY_ANGLE) children();
  }

  module display_profile() {
    display_anchor() square([DISPLAY_PCB_H, DISPLAY_D]);
  }

  rotate([90,0,90])difference() {
    linear_extrude(DISPLAY_PCB_W+W_WALL*2, center=true) offset(W_WALL)hull(){
      display_profile();
      translate([-DISPLAY_PCB_H/2,0])square([DISPLAY_PCB_H, 1]);
      square([DISPLAY_PCB_H/2, W_WALL]);
    }
    linear_extrude(DISPLAY_PCB_W, center=true){
      hull(){ 
        display_profile();
        translate([0,-1])square([7,1]);
      }
    }
    linear_extrude(100,center=true)translate([-100,-100])square([200,100]);
    linear_extrude(DISPLAY_W,center=true){
      display_anchor(){
        translate([0,DISPLAY_D])polygon([
          [OFFSET_DISPLAY_Y,-1],
          [OFFSET_DISPLAY_Y,0.4],
          [OFFSET_DISPLAY_Y-W_WALL, W_WALL+0.4],
          [OFFSET_DISPLAY_Y+DISPLAY_H+W_WALL, W_WALL+0.4],
          [OFFSET_DISPLAY_Y+DISPLAY_H,0.4],
          [OFFSET_DISPLAY_Y+DISPLAY_H,-1],
        ]);
      }
    }
  }
}

let($fn=72){
  display_mount();
}

