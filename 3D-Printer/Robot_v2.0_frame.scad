module show_wheels(){
    translate([0,7,8]){
        rotate([0,90,0]){
            cylinder(h = 5, r1 = 7, r2 = 7, $fn = 100);
        }
    }
}

module mirror(wt, mw){

    // holder walls
    translate([0,0,7+wt]){
        cube(size = [mw+4, wt, 3]);
        translate([0, 6.2+wt, 0]){
            cube(size = [21, wt, 3]);
        }
    }

    // motor holder right
    difference(){
        cube(size = [mw+4, 6.2+2*wt, 7+wt]);
        // motor
        translate([0,3.1+wt,7+wt]){
            rotate([0,90,0]){
                cylinder(h = 19+4, r1 = 3.1, r2 = 3.1, $fn = 100);
            }
        }
    }
}

module main(wt, mw){

    bh = 23;
    rb = 0.5*25*0.58;
    rh = 1.4; //0.5*25*(0.09);
    // base
    difference(){
        union(){
            cube(size = [mw+4, bh-0.5, wt]);
            translate([0.5*(mw+4), bh-0.5, 0]){
                 cylinder(h = wt, r1 = 0.5*bh, r2 = 0.5*bh, $fn = 100);
            }
        }
        translate([((mw+4)/2), 23, 0]){
            rotate([0,0,90]){
                translate([rb, 0, 0]){
                    cylinder(h = 1, r1 = rh, r2 = rh, $fn = 100);
                }
                /*translate([-rb, 0, 0]){
                    cylinder(h = 2, r1 = rh, r2 = rh, $fn = 100);
                }*/
            }
        }
    }

    mirror(wt, mw);

    rotate([0,0,180]){
        translate([-(mw+4),-2*(6.2)-3*wt,0]){
            mirror(wt, mw);
        }
    }
    l = 1;
    w = 8;
    h = 10;
    
    translate([wt, 2*6.2+3*wt+8, wt]){
        //Start with an extruded triangle
        rotate(a=[0,0,180]){
            polyhedron(
               points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
               faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]
               );
        }
    }
    translate([23, 2*6.2+3*wt+8, wt]){
        //Start with an extruded triangle
        rotate(a=[0,0,180]){
        polyhedron(
               points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
               faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]
               );
        }
    }
    
}

module caster(){
    
rb = 0.5*25*0.58;
re = 0.5*25*(0.8-0.53);
rh = 0.5*25*(0.09);
h1 = 0.03*25;

rball = 0.5*25*0.5;

    difference(){
        union(){
            cylinder(h = h1, r1 = rb, r2 = rb, $fn = 100);

            translate([rb, 0, 0]){
                cylinder(h = h1, r1 = re, r2 = re, $fn = 100);
            }

            translate([-rb, 0, 0]){
                cylinder(h = h1, r1 = re, r2 = re, $fn = 100);
            }
        }
        union(){
            translate([rb, 0, 0]){
                cylinder(h = h1, r1 = rh, r2 = rh, $fn = 100);
            }
            translate([-rb, 0, 0]){
                cylinder(h = h1, r1 = rh, r2 = rh, $fn = 100);
            }
        }
    }

    translate([0,0,h1+rball]){
        sphere(rball, $fn=100);
    }
}

module tie_hole (x, w){
    union(){
        translate([x-w,0,1]){
            cube(size = [2*w, 2*6.3+3*1.0, 1.2]);
        }
        translate([x-w,0,11-1.2]){
            cube(size = [2*w, 2*6.3+3*1.0, 1.2]);
        }
        //remove 
        translate([(23-7)/2,(2*6.2+3*1)-3.5,1]){
            cube(size = [7, 4, 2.5]);
        }
    } 
}

module show_parts(){
    show_wheels();
    translate([28,6.2+1.0,0]){
        show_wheels();
    }

    translate([5,3.1,0]){
        mw = 19;
        difference(){
            main(1.0, mw);
            tie_hole(6, 1.5);
            tie_hole(23-6, 1.5);
        }
        translate([((mw+4)/2), 23, 1]){
            rotate([0,0,90]){
                caster();
            }
        }
    }
}
//show_parts();


difference(){
    main(1.0, 19);
    tie_hole(5.5, 1.5);
    tie_hole(23-5.5, 1.5);
    
}
