use <bartscad/stack.scad>

D_NECK=50;
R_WHEEL=18;
W_WHEEL=10;

D_NOTCH=20;

SPACING=0.5;

W_BEARING=4;

module wheel_2d(){
  offset(1)difference(){
    circle(r=R_WHEEL-1); 
    translate([0,R_WHEEL+D_NOTCH/3])circle(d=D_NOTCH+2);
  }
}

module magnet_2d(){
  offset(0.1)square([5,4], center=true);
}

module wheel_cutout_2d() {
  circle(r=R_WHEEL+SPACING);
}

module bearing_outer_2d(){
  circle(d=18);
}

module bearing_inner_2d(){
  circle(d=12);
}

module bearing_2d() {
  // type 6701
  difference(){
    bearing_outer_2d();
    bearing_inner_2d();
  }
}

module mounting_bracket_ring_2d() {
  difference(){
    offset(4-SPACING) bearing_outer_2d();
    bearing_outer_2d();
  }
}

module mounting_bracket_foot_2d() {
  difference(){
    union(){
      offset(4-SPACING) bearing_outer_2d();
      translate([-21/2,-R_WHEEL-SPACING])square([21,R_WHEEL+SPACING]);
    }
    bearing_outer_2d();
  }
}

module wheel(){
  stack([["bearing", W_BEARING],["spacer",SPACING],["body",5]]){
    difference(){
      wheel_2d();
      layer(["bearing","spacer"]) difference(){
        offset(4) bearing_outer_2d();
        layer("bearing") offset(0.05)bearing_inner_2d();
        layer("spacer") offset(1) bearing_inner_2d();
      }
      magnet_2d();
    }
  }
}

module foot() {
  translate([0,0,-5-SPACING]) stack([["foot",5], ["spacer", SPACING], ["bearing", W_BEARING]]){
    difference(){
      union(){
        offset(4-SPACING) bearing_outer_2d();
        layer("foot")translate([-(18+8-2*SPACING)/2,-R_WHEEL-SPACING])square([18+8-2*SPACING,R_WHEEL+SPACING]);
      }
      layer("spacer")offset(-1)bearing_outer_2d();
      layer("bearing")bearing_outer_2d();
      magnet_2d();
    }
  }
}

module neck2d() {
  scale([1,.8])offset(4)difference(){
    circle(r=D_NECK/2-4);
    translate([-50,3])square(100);
  }
}

translate([0,0])rotate([0,0,45]){
  foot();
  wheel();
}

*linear_extrude(100, center=true)neck2d();
