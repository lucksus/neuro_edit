function gerade(x,y){
  return 0.5*x-y+1;
}

function step(i){
  x = RandomGenerator.uniform(-10,10);
  y = RandomGenerator.uniform(-10,10);
  target = [gerade(x,y)];
  input = [x,y,1];
  output = mlp.forward_run(input);
  print(i, ": error before = ",target[0] - output[0]);
  mlp.backward_run(target);
  mlp.update_weights(0.002);
}

function train(){
  for(var i=0;i<100;i++){
   step(i);
  }
}

mlp = new MLP(3,1,1);
show(mlp)

