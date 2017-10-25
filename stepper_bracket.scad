module stepper_bracket() {
    module make_body() {
        thickness = 5;
        nema_w = 56.4;
        nema_h = nema_w;
        nema_d = 54;
        union () {
            difference () {
                cube([nema_w+2*thickness, nema_d+thickness, nema_h+2*thickness], false);
                translate ([thickness, thickness, thickness]) {
                    cube([nema_w, nema_d+2*thickness, nema_h+2*thickness], false);
                }
                translate([0,0,nema_h+2*thickness]) {
                    rotate([-45, 0, 0]) {
                        cube([100, 100, 100], false);
                    }
                }
            }
            cube([nema_w+50+2*thickness, nema_d+thickness, thickness], false); 
        }
    }
    module make_front_material() {
        translate() {
            rotate() {
                cylinder
        }
    }
    module make_base_material() {
    }
    make_body();
    remove_front_material();
    remove_base_material();
}
stepper_bracket();