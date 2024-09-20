
module nut(height, width){
  let ($fn=6, d=width/sqrt(3/4)){
    cylinder(h=height, d=d, center=true);
  }
}

module captive_nut_m5(depth=50, $tolerance=0) {
  hull(){
    nut(4+2*$tolerance,8+2*$tolerance);
    translate([depth,0,0])nut(4+2*$tolerance,8+2*$tolerance);
  }
}

module captive_nut_screw_hole_m5(depth_nut=50, depth_top=100, depth_bottom=10, $tolerance=0) {
  translate([0,0,-depth_bottom])cylinder(d=5+2*$tolerance, h=depth_top+depth_bottom);
  captive_nut_m5(depth_nut, $tolerance);
}

captive_nut_screw_hole_m5($tolerance=.1);
