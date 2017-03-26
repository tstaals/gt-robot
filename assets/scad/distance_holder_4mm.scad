$fn = 220;
difference () {
    cylinder(r=2.5,h=4);
    translate([0,0,-2]) {
        cylinder(r=1.5,h=12);
    }
}