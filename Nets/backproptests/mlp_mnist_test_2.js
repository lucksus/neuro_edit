data = new MNISTData();
data.load_images("/Users/nico/t10k-images.idx");
data.load_labels("/Users/nico/t10k-labels.idx");
image_size = data.number_of_rows() * data.number_of_columns();
mlp = new MLP(image_size,image_size*5,10);
