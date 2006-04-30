camera {
   perspective
   location <5, 5, -5>
   sky <0, 1, 0>
   direction <0, 0, 1>
   right <1.33333, 0, 0>
   up <0, 1, 0>
   look_at <0, 0, 0>
}

light_source {
   //*PMName Main light
   <1, 3, 1>, rgb <1, 1, 1>
}

sphere {
   //*PMName Sphere
   <0, 0, 0>, 1
   
   pigment {
      color rgbft <0.35686, 0.79216, 0.91373, 0.3, 0.4>
   }
   
   finish {
      ambient rgb <0.333333, 0.619608, 0.647059>
      diffuse 0.6
      brilliance 1
      specular 0.8
   }
   scale 1
   rotate <0, 0, 0>
   translate <0, 0, 0>
   hollow
}