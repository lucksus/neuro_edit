sim = simulation();

signal_file = "/Users/nico/Neuro/signal_unverzerrt.txt";
time_offset = 10;

function random_indices(count, max){
	arr = new Array();
	while(arr.length < count){
		x = RandomGenerator.uniform(0,max);
		x -= x%1;
		arr[arr.length] = x;
	}
	return arr;
}

function setup(){
	group = new Group();
	read_out = new ReadOut();
	input = new Samples();
	output = new Samples();
	signal = new PhotossSignalImporter(signal_file);
	read_out.position.x = 100
	input.position.x = -100;
	output.position.x = 150;
	group.create_2d_grid(20,10,40);
	group.create_connections(0.3,65);
	read_out.connect_with_group(group);
	read_out.init_weights_random_uniform(-1,1);
	read_out.add_output(output);
	input.read_from_photoss_signal_file(signal_file);



	indices = random_indices(10, 20*10);
	for(var i=0;i<indices.length;i++){
		index = indices[i];
		ci = new CurrentInducer(group.objects()[index].dendrites_root());
		input.add_current_inducer(ci);
		ci.set_active(true);
	}
}

function training(){
	sim.reset();
	var i = 0;
	while(sim.time < 1600){
		sim.run_one_step(0.1);
		if(i==20) print(sim.time);
		bit_time = sim.time - time_offset;
		if(bit_time > 0){
			bit = signal.bit_at_time(bit_time);
			if(bit) read_out.learn(0.8,0.05);
			else read_out.learn(-0.8,0.05);
			if(i==20) print(bit);
		}
		if(i==20) i=0;
		i++;
	}
}

function test(){
	sim.reset();
	var i = 0;
	while(sim.time < 1600){
		sim.run_one_step(0.1);
		if(i==20) print(sim.time);
		if(i==20) i=0;
		i++;
	}
}




