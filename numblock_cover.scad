use <BOSL/transforms.scad>

module cover_2d(){
        move([0,7.5])square([76, 87.5]);
        square([68.5, 95]);
        move([68.5,7.5,0]) circle(r=7.5);
}

module screw_posts_2d(){
    pos1 = [59,7];
    pos2 = [19.3,58];

    move(pos1) difference(){
        union(){
            circle(d=6);
            move([-3, -pos1.y])square([6,pos1.y]);
        }
        circle(d=2);
    }

    move(pos2)difference(){
        union(){
            circle(d=6);
            move([-pos2.x, -3])square([pos2.x,6]);
        }
        circle(d=2);
    }
}

module pico_mount_2d(){
    positions = [[0,0],[11.4,0],[0,46.6],[11.4,46.6]];
    for (pos = positions) {
        move(pos)difference(){
            circle(d=3.8);
            circle(d=1.5);
        }
    }
}

module wall_2d(){
    difference(){
        cover_2d();
        offset(r=-3)cover_2d();
    }
}

module pot_holes(){
    d=7;
    positions=[[18,95-18],[58,95-18]];
    for (pos = positions){
        move(pos)circle(d=d);
    }
}

module midi_port_inner(edge=false) {
    move([76,0,7.5])
    yrot(-90)zmove(-1)linear_extrude(28){
        circle(d=15);
        if(edge){
            xmove(-15)ymove(-5)square([15,10]);
        }
    }
}

module usb_port_inner(){
    dimensions = [10.6,12.04];
    move([76,0,0])
    yrot(-90)zmove(-1)linear_extrude(5)square(dimensions);
}

height=15;

difference(){
    union(){
        zmove(height)linear_extrude(3)difference(){
            cover_2d();
            pot_holes();
        }

        linear_extrude(height)screw_posts_2d();
        move([32,10,height-5]) linear_extrude(5)pico_mount_2d();
        linear_extrude(height)wall_2d();
    }
    ymove(95-18-8-8)midi_port_inner(true);
    ymove(35)usb_port_inner();

    ymove(34)cube([5,6,8]);
}
