#!/bin/bash
rm  ./Examples/camera/bin/data/dnn/*.weights 
rm  ./Examples/osc_sender/bin/data/dnn/*.weights 
rm  ./Examples/single_image/bin/data/dnn/*.weights 
rm  ./Examples/annotation/bin/data/dnn/*.weights 
rm  ./Examples/single_image/bin/data/dnn/*.cfg
rm  ./Examples/camera/bin/data/dnn/*.cfg
rm  ./Examples/annotation/bin/data/dnn/*.cfg
rm -fR ./Examples/camera/bin/camera*.app
rm -fR ./Examples/single_image/bin/single_image*.app
rm -fR ./Examples/osc_sender/bin/osc_sender*.app
rm -fR ./Examples/annotation/bin/annotation*.app
echo "done"