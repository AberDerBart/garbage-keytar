use <bartscad/poly.scad>;

HOLE_POSITIONS = [[4,4], [66,4], [66,86], [4, 86]];

module pcb(with_holes=false){
  difference(){
    offset(4) offset(-4) square([70, 90]);
    if(with_holes)
      for(pos = HOLE_POSITIONS)
      translate(pos)
      circle(d=3.2);
  }
}

module connector_holes_left(spacing=0, top_open=false) {
  translate([0,0,1.6]){
    // PS/2
    translate([0,68.9,6.5]) rotate([0,90,0]) cylinder(h=10, d=10, center=true);
    // USB-A
    translate([0,47.25,3.5+1.5+(top_open?20:0)]) cube([10, 14.3+2*spacing, 7+2*spacing+(top_open?40:0)], center=true);
  }
}

module connector_holes_right(spacing=0, top_open=false) {
  translate([0,0,1.6]){
    // USB-B
    translate([70,74.2985,5.3]) cube([10, 12.8, 10.6], center=true);
    // MIDI
    translate([70,90-38.9425, 19.5/2]) rotate([0,90,0])cylinder(d=15, h=10, center=true);
    // Pedal
    translate([70,28.9596-7.23,6.5]) rotate([0,90,0]) cylinder(d=11.2+2*spacing, h=10, center=true);
  }
}

function h_pcb() = 1.6 + 19.5;

module pcb_hole(spacing=0, top_open=false) {
  translate([0,0,-3]) linear_extrude(1.6+19.5+3+(top_open?20:0)) {
    offset(spacing) pcb();
  }
  connector_holes_left(spacing,top_open);
  connector_holes_right(spacing,top_open);
}

module at_pcb_screw_positions() {
  for(pos=HOLE_POSITIONS) {
    translate(pos)children();
  }
}

module pcb_feet(spacing=0,d_hole = 2.8) {
  translate([0,0,-3])
    linear_extrude(3)
    for(i = [0:3])
  {
    translate(HOLE_POSITIONS[i])
      rotate([0,0,i*90])
      difference()
    {
      poly([
        [-4.1-spacing,-4.1-spacing],
        [6,-4.1-spacing],
        fillet([6,6],6),
        [-4.1-spacing,6],
      ]);
      circle(d=d_hole);
    }
  }
}
$fn=120;

//linear_extrude(1.6)pcb(true);

#pcb_hole(0.1);

pcb_feet(0.1);
