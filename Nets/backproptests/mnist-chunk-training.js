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

function train_chunk(count){
   for(var i=0;i<data.number_of_images();i++) {
	print("learning image ", i, "...");
	out = mlp.forward_run(data.image_raw(i));
	print(out);
	arr = [0,0,0,0,0,0,0,0,0,0];
	arr[data.label(i)] = 1;
	mlp.backward_run(arr);
	mlp.add_errors_to_average_update_vector(0.01);
	if((i % count) == 0){
		//print("update vector:");
		//print(mlp.print_average_update_vector());
		mlp.apply_average_update_vector();
		print("update vector applied.");
		mlp.reset_average_update_vector();
	}
    }
}


train_chunk(20);
mlp.save("/Users/nico/mnist-sigmoid-chunk-training-1.mlp")
train_chunk(20);
mlp.save("/Users/nico/mnist-sigmoid-chunk-training-2.mlp")
train_chunk(20);
mlp.save("/Users/nico/mnist-sigmoid-chunk-training-3.mlp")
