
$fn = 100;
// maten
//
// 25 * 25 mm
// 
union() {
    case();
    pw(-10,9,0);
    rotate([0,180,0]) {
        pw(-40,9,-3);
    }
}
module case() {
    union() {    
        difference () {
            cube([30,30,15]);
            translate([2,2,5]) { 
                cube([26,26,18]);
            }
            translate([5,-1,10]) {
                cube([20,4,15]);
            }
        }
        difference() {
            union() {
                difference () {
                    translate([2,22,5]) {
                        cube ([26,6,3]);
                    }
                    
                    translate([4,25,5]) {
                        cylinder(r=1,h=10, center=false);
                    }
                    translate([26,25,5]) {
                        cylinder(r=1,h=10, center=false);
                    }
                }
                difference(){
                    translate([2,7,5]) {
                        cube ([26,6,3]);
                    }
                    translate([4,10,5]) {
                        cylinder(r=1,h=10, center=false);
                    }
                    translate([26,10,5]) {
                        cylinder(r=1,h=10, center=false);
                    }   
                }
            }
            translate([7,5,5]) {
                cube ([16,24,4]);
            }
        }
    }
}
module pw (x,y,z) {
    difference () {
        translate([x,y,z]) 
        cube([10,12,3]);
        translate ([x-1,y+4,z-1])
        cube([6,4,5]);
        translate([x+5,y+6,z-2])
        cylinder(r=2,h=10, center=false);
    }
}