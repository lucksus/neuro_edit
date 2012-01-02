function gerade(x,y){
  return 0.5*x-y+1;
}

net = simulation.network;
group = net.object_by_name("group");
input1 = group.object_by_name("input1");
input2 = group.object_by_name("input2");
output = group.object_by_name("output");

input1.use_constant_output = true;
input2.use_constant_output = true;

for(var i=0;i<100;i++){
  x = RandomGenerator.uniform(-10,10);
  y = RandomGenerator.uniform(-10,10);
  target = gerade(x,y);
  input1.constant_output = x;
  input2.constant_output = y;
  output.update(1);
  out = output.output;
  print(i, ": error before = ",target - out);
  group.do_backprop(target,output);
  output.update(1);
  out = output.output;
  print(i, ": error after = ",target - out);
}