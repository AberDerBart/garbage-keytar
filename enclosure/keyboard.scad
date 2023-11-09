R_CORNER = 16.25/2;

W_BOTTOM = 138.6;
W_TOP = 136;
H_KEYBOARD=19.3;

function h_keyboard() = H_KEYBOARD;

module keyboard(anchor="center", open_top=false){
  offset=anchor=="left" ? [50,0]: anchor=="right" ? [-50:0]: [0,0];
  translate(offset){
    hull(){
      linear_extrude(0.2)
        offset(R_CORNER)
        offset(-R_CORNER)
        square([100,W_BOTTOM],center=true);
      translate([0,0,H_KEYBOARD-0.2])
        linear_extrude(0.2)
        offset(R_CORNER)
        offset(-R_CORNER)
        square([100-5.2,W_TOP],center=true);
    }
    if(open_top)
      linear_extrude(2*H_KEYBOARD)
      offset(R_CORNER)
      offset(-R_CORNER)
      square([100-5.2,W_TOP], center=true);
  }
}

function keyboard_top_max_x() = (W_BOTTOM-W_TOP)/2;

module keyboard_edge_top_left(){
  translate([keyboard_top_max_x(),0,H_KEYBOARD]) children();
}

function keyboard_overlap_top() = 5;

module cable_channel_cross(){
  translate([3,0,0]){
    circle(d=6);
    translate([-20,0,0])square([40,6], center=true);
  }
}
