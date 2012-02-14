group = new Group();
read_out = new ReadOut();
input = new Samples();
output = new Samples();
read_out.position.x = 100
input.position.x = -100;
output.position.x = 150;
group.create_2d_grid(20,10,40);
group.create_connections(0.3,65);
read_out.connect_with_group(group);
input.read_from_photoss_signal_file("/Users/nico/Neuro/signal_unverzerrt.txt");

function random_indices(count, max){
	arr = new Array();
	while(arr.length < count){
		x = RandomGenerator.uniform(0,max);
		x -= x%1;
		arr[arr.length] = x;
	}
	return arr;
}

indices = random_indices(10, 20*10);
for(var i=0;i<indices.length;i++){
	index = indices[i];
	ci = new CurrentInducer(group.objects()[index].dendrites_root());
	input.add_current_inducer(ci);
}
