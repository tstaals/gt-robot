$fn = 100;

difference () {
    union () {
       bracket();
    }
    union () {
        holder();
    }
}

module bracket() {
    
    difference () {
        union () {
            cube ([55, 29, 2]);
                translate ([0, -2, 0]) 
                cube ([80, 2, 10]);
           }
        union() {
            translate ([27.5,0,6])
            rotate([90, 0, 0]) {
                cylinder (h=3, r=2.3);
            }
        }
    }
    difference () {
        union () {
            translate ([27.5,1,6])
            rotate([90, 0, 0]) 
            cylinder (h=1, r=3.5);
        }
        union () {
            translate ([27.5,2,6])
            rotate([90, 0, 0]) 
            cylinder (h=2, r=0.5);
        }
    }
    translate ([24,1,2])
    rotate([90, 0, 0]) 
    cube ([7, 4, 1]);
    rotate([-90,-90,0])
    translate ([0,0,0])
    prism(2,10,15);
}

module holder () {
    translate ([14, 15, 0]) 
        cylinder  (h=2, r=8.5 , $fs=0.1 );
    translate ([40, 15, 0]) 
        cylinder  (h=2, r=8.5 , $fs=0.1 ); // cil
    translate ([22, 5, 0]) 
        cube ([10, 4, 2]);  // crystal
    translate ([6, 6, 0]) 
        cylinder  (h=2, r=1 , $fs=0.1 );
    translate ([6, 23, 0]) 
        cylinder  (h=2, r=1.2 , $fs=0.1 );
    translate ([47, 6, 0]) 
        cylinder  (h=2, r=1.2 , $fs=0.1 );
    translate ([48, 24, 0]) 
        cylinder  (h=2, r=1 , $fs=0.1 );
}

// Draw a prism based on a right triangle
// l:length, w: width, h: height
// from https://github.com/dannystaple/OpenSCAD-Parts-Library/blob/master/prism.scad
module prism(l, w, h) {
       polyhedron(points=[
               [0,0,h],           // 0    front top corner
               [0,0,0],[w,0,0],   // 1, 2 front left & right bottom corners
               [0,l,h],           // 3    back top corner
               [0,l,0],[w,l,0]    // 4, 5 back left & right bottom corners
       ], faces=[ // points for all faces must be ordered clockwise when looking in
               [0,2,1],    // top face
               [3,4,5],    // base face
               [0,1,4,3],  // h face
               [1,2,5,4],  // w face
               [0,3,5,2],  // hypotenuse face
       ]);
}