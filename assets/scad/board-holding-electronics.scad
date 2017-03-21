$fn = 100;

difference() {
    cube ([140,35,2.5]);
        translate([35, 2.5, 0]) {
            cube ([12,4,2.5]);
        }
        translate ([100,2.5,0]) {
            cube ([12,4,2.5]);
        }
}