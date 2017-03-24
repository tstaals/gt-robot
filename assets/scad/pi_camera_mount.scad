
$fn = 100;
// maten
//
// 25 * 25 mm
// 
union() {
    case();
    pw(-10,10,0);
    pw(30,10,0);
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
        difference () {
            translate([2,22,5]) {
                cube ([26,6,3]);
            }
            translate([6,25,5]) {
                cylinder(r=0.5,h=10, center=false);
            }
            translate([24,25,5]) {
                cylinder(r=0.5,h=10, center=false);
            }
        }
        difference(){
            translate([2,7,5]) {
                cube ([26,6,3]);
            }
            translate([6,10,5]) {
                cylinder(r=0.5,h=10, center=false);
            }
            translate([24,10,5]) {
                cylinder(r=0.5,h=10, center=false);
            }   
        }
    }
}
module pw (x,y,z) {
    difference () {
        translate([x,y,z]) 
        cube([10,10,3]);
        translate([x+5,y+5,z-2])
        cylinder(r=2,h=10, center=false);
    }
}