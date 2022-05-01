cd ../include
protoc proto.proto --cpp_out=.
mv  proto.pb.cc ../src
cd ../src
