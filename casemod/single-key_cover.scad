use <BOSL/transforms.scad>

$fn=72;
hull(){
    linear_extrude(4)square(12, center=true);
    linear_extrude(1)square(18, center=true);
}
*linear_extrude(2)square(18,center=true);
color("red")zflip()linear_extrude(4){
    difference(){
        square(18,center=true);
        square(14,center=true);
    }
    difference(){
        square([7,6.5],center=true);
        square([2,7],center=true);
        circle(d=2.6);
    }
}
