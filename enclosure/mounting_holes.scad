module mounting_holes(offs, d=2.8) {
  for(x=[offs.x, -offs.x]) {
    for(y=[offs.y, -offs.y]) {
      translate([x,y])circle(d=d);
    }
  }
}
