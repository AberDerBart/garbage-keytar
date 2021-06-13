use <BOSL/transforms.scad>


y_rows = [95.5, 77, 57, 40, 19];

function wave_edge_xend(angle = 45, r = 11, h = 9) = tan(angle/2) * r * 2 + 1/tan(angle)*h;

module wave_edge_thingy(r=11, h = 9) {
    angle=45;
    move([0,-r]) difference(){
        polygon(points = [
            [0,0],
            [0,r+1],
            [tan(angle) * r+1,r+1],
        ]);
        circle(r=r);
    }
    x_end = wave_edge_xend(angle,r,h);
    move([x_end, r-h]){
        circle(r=r);
        zrot(180-angle)square([h/sin(angle), r]);
    }
}

module keyboard_cover_left_2d(){
    polygon(points=[
        [0, 7.5],
        [0, y_rows[0]],
        [38, y_rows[0]],
        [38, y_rows[1]],
        [28, y_rows[1]],
        [28, y_rows[2]],
        [52, y_rows[2]],
        [52, y_rows[3]],
        [23, y_rows[3]],
        [23, y_rows[4]],
        [131, y_rows[4]],
        [131, -9],
        [45, -9],
        [37.5, 0],
        [7.5,0],
    ]);
    move([7.5, 7.5])circle(d=15);

    move([46-wave_edge_xend(), 0])wave_edge_thingy();
}


module keyboard_cover_right_2d(){
    polygon(points=[
        [0,0],
        [0, y_rows[0]],
        [-38, y_rows[0]],
        [-38, y_rows[1]],
        [-29, y_rows[1]],
        [-29, y_rows[2]],
        [-62, y_rows[2]],
        [-62, y_rows[3]],
        [-53, y_rows[3]],
        [-53, y_rows[4]],
        [-155.5, y_rows[4]],
        [-155.5, -9],
        [-45.5, -9],
        [-36.5, 0],
    ]);

    move([-46+wave_edge_xend(), 0])xflip()wave_edge_thingy();
}

module left_with_handle(){
    keyboard_cover_left_2d();
    xmove(-160)square([160,35]);
}

/* linear_extrude(2)keyboard_cover_left_2d();

zflip()linear_extrude(4)difference(){
    keyboard_cover_left_2d();
    offset(r=-2)keyboard_cover_left_2d();
}
*/

//keyboard_cover_left_2d();
left_with_handle();
//xmove(-10)keyboard_cover_right_2d();
