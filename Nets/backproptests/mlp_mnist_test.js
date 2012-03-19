data = new MNISTData();
data.load_images("/Users/nico/t10k-images.idx");
data.load_labels("/Users/nico/t10k-labels.idx");

sorted_indices = [];
for(var i=0;i<=9;i++) sorted_indices[i] = [];
for(var i=0;i<data.number_of_images();i++){
  length = sorted_indices[data.label(i)].length;
  sorted_indices[data.label(i)][length] = i;
}


image_size = data.number_of_rows() * data.number_of_columns();
mlp = new MLP(image_size,image_size*5,10);
//show(mlp)

i=0;
function step(i){
//for(var i=0;i<data.number_of_images();i++){
	print("learning image ", i, "...");
	i++;
	out = mlp.forward_run(data.image_raw(i));
	print(out);
	arr = [0,0,0,0,0,0,0,0,0,0];
	arr[data.label(i)] = 1;
	mlp.backward_run(arr);
	mlp.update_weights(0.02);
}

function train(count){
for(var i=0;i<count;i++) step(i)
}

function train_distinct(number){
  for(var i=0;i<sorted_indices[number].length;i++){
                   index = sorted_indices[number][i];
                   print(i, ": learning image ", index, "...");
	i++;
	out = mlp.forward_run(data.image_raw(i));
	print(out);
	arr = [0,0,0,0,0,0,0,0,0,0];
	arr[data.label(index)] = 1;
	mlp.backward_run(arr);
	mlp.update_weights(0.008);
  }
}

function antwort(arr){
  var max; var max_index;
  for(var i=0;i<arr.length;i++)
    if(i==0 || arr[i]>max){
       max = arr[i];
       max_index = i;
    }
  return max_index;
}

train_distinct(1)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(1)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(2)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(2)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(1)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(1)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(2)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(2)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
train_distinct(1)
mlp.save("/Users/nico/mnist-sigmoid-3-runs.mlp")
