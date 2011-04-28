<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!DOCTYPE boost_serialization>
<boost_serialization signature="serialization::archive" version="7">
<network class_id="6" tracking_level="0" version="0">
	<SimulationObjects class_id="7" tracking_level="0" version="0">
		<count>17</count>
		<item_version>0</item_version>
		<item class_id="3" tracking_level="1" version="0" object_id="_0">
			<SpikeEmitter class_id="8" tracking_level="0" version="0">
				<SpatialObject class_id="9" tracking_level="0" version="0">
					<SimulationObject class_id="10" tracking_level="0" version="0">
						<m_neuron class_id="0" tracking_level="1" version="0" object_id="_1">
							<SpatialObject>
								<SimulationObject>
									<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
								</SimulationObject>
								<Position class_id="11" tracking_level="0" version="0">
									<x>24.742388096198397</x>
									<y>0</y>
									<z>2.6676121113457043</z>
								</Position>
								<is_user_movable>1</is_user_movable>
							</SpatialObject>
							<NeuronModel class_id="1" tracking_level="1" version="0" object_id="_2">
								<NeuronModel class_id="12" tracking_level="0" version="0">
									<SpatialObject>
										<SimulationObject>
											<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
										</SimulationObject>
										<Position>
											<x>24.742388096198397</x>
											<y>0</y>
											<z>2.6676121113457043</z>
										</Position>
										<is_user_movable>1</is_user_movable>
									</SpatialObject>
									<is_spiking>0</is_spiking>
								</NeuronModel>
								<a>0.02</a>
								<b>0.20000000000000001</b>
								<c>-65</c>
								<d>8</d>
								<m_u>-13.132290094322693</m_u>
								<m_v>-71.375397714994975</m_v>
							</NeuronModel>
							<DendridesRoot class_id="5" tracking_level="1" version="0" object_id="_3">
								<SpatialObject>
									<SimulationObject>
										<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
									</SimulationObject>
									<Position>
										<x>16.742388096198397</x>
										<y>0</y>
										<z>2.6676121113457043</z>
									</Position>
									<is_user_movable>0</is_user_movable>
								</SpatialObject>
								<AddedCurrent>0</AddedCurrent>
								<Parent class_id="-1"></Parent>
								<Children class_id="13" tracking_level="0" version="0">
									<count>0</count>
									<item_version>0</item_version>
								</Children>
								<IncomingSynapses class_id="14" tracking_level="0" version="0">
									<count>2</count>
									<item_version>0</item_version>
									<item class_id="4" tracking_level="1" version="0" object_id="_4">
										<SpikeReceiver class_id="15" tracking_level="0" version="0">
											<SpatialObject>
												<SimulationObject>
													<m_neuron class_id_reference="0" object_id="_5">
														<SpatialObject>
															<SimulationObject>
																<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
															</SimulationObject>
															<Position>
																<x>-65.0815222128236</x>
																<y>0</y>
																<z>37.207065087586557</z>
															</Position>
															<is_user_movable>1</is_user_movable>
														</SpatialObject>
														<NeuronModel class_id_reference="1" object_id="_6">
															<NeuronModel>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>0</x>
																		<y>0</y>
																		<z>0</z>
																	</Position>
																	<is_user_movable>1</is_user_movable>
																</SpatialObject>
																<is_spiking>0</is_spiking>
															</NeuronModel>
															<a>0.02</a>
															<b>0.20000000000000001</b>
															<c>-65</c>
															<d>8</d>
															<m_u>-11.357598604918165</m_u>
															<m_v>-73.64621267207373</m_v>
														</NeuronModel>
														<DendridesRoot class_id_reference="5" object_id="_7">
															<SpatialObject>
																<SimulationObject>
																	<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
																</SimulationObject>
																<Position>
																	<x>-73.0815222128236</x>
																	<y>0</y>
																	<z>37.207065087586557</z>
																</Position>
																<is_user_movable>0</is_user_movable>
															</SpatialObject>
															<AddedCurrent>0</AddedCurrent>
															<Parent class_id="-1"></Parent>
															<Children>
																<count>0</count>
																<item_version>0</item_version>
															</Children>
															<IncomingSynapses>
																<count>0</count>
																<item_version>0</item_version>
															</IncomingSynapses>
														</DendridesRoot>
														<AxonRoot class_id_reference="3" object_id="_8">
															<SpikeEmitter>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>-57.0815222128236</x>
																		<y>0</y>
																		<z>37.207065087586557</z>
																	</Position>
																	<is_user_movable>0</is_user_movable>
																</SpatialObject>
																<is_spiking>0</is_spiking>
															</SpikeEmitter>
															<SpikeReceiver>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>-57.0815222128236</x>
																		<y>0</y>
																		<z>37.207065087586557</z>
																	</Position>
																	<is_user_movable>0</is_user_movable>
																</SpatialObject>
																<spike_received>0</spike_received>
																<IncomingAxons class_id="16" tracking_level="0" version="0">
																	<count>0</count>
																	<item_version>0</item_version>
																</IncomingAxons>
															</SpikeReceiver>
															<Receivers>
																<count>1</count>
																<item_version>0</item_version>
																<item class_id="2" tracking_level="1" version="0" object_id="_9">
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_5"></m_neuron>
																	</SimulationObject>
																	<Emitter class_id_reference="3" object_id_reference="_8"></Emitter>
																	<Receiver class_id_reference="4" object_id_reference="_4"></Receiver>
																	<Speed>55</Speed>
																	<ActionPotentials>
																		<count>0</count>
																		<item_version>0</item_version>
																	</ActionPotentials>
																</item>
															</Receivers>
														</AxonRoot>
													</m_neuron>
												</SimulationObject>
												<Position>
													<x>16.742388096198397</x>
													<y>0</y>
													<z>2.6676121113457043</z>
												</Position>
												<is_user_movable>0</is_user_movable>
											</SpatialObject>
											<spike_received>0</spike_received>
											<IncomingAxons>
												<count>1</count>
												<item_version>0</item_version>
												<item class_id_reference="2" object_id_reference="_9"></item>
											</IncomingAxons>
										</SpikeReceiver>
										<PostsynapticDendriticNode class_id_reference="5" object_id_reference="_3"></PostsynapticDendriticNode>
										<Weight>30</Weight>
										<Time_constant>80</Time_constant>
										<ActivePotentials>
											<count>0</count>
											<item_version>0</item_version>
										</ActivePotentials>
									</item>
									<item class_id_reference="4" object_id="_10">
										<SpikeReceiver>
											<SpatialObject>
												<SimulationObject>
													<m_neuron class_id_reference="0" object_id="_11">
														<SpatialObject>
															<SimulationObject>
																<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
															</SimulationObject>
															<Position>
																<x>-67.149826402533748</x>
																<y>0</y>
																<z>-32.525359321742584</z>
															</Position>
															<is_user_movable>1</is_user_movable>
														</SpatialObject>
														<NeuronModel class_id_reference="1" object_id="_12">
															<NeuronModel>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>0</x>
																		<y>0</y>
																		<z>0</z>
																	</Position>
																	<is_user_movable>1</is_user_movable>
																</SpatialObject>
																<is_spiking>0</is_spiking>
															</NeuronModel>
															<a>0.02</a>
															<b>0.20000000000000001</b>
															<c>-65</c>
															<d>8</d>
															<m_u>-11.357598604918165</m_u>
															<m_v>-73.64621267207373</m_v>
														</NeuronModel>
														<DendridesRoot class_id_reference="5" object_id="_13">
															<SpatialObject>
																<SimulationObject>
																	<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
																</SimulationObject>
																<Position>
																	<x>-75.149826402533748</x>
																	<y>0</y>
																	<z>-32.525359321742584</z>
																</Position>
																<is_user_movable>0</is_user_movable>
															</SpatialObject>
															<AddedCurrent>0</AddedCurrent>
															<Parent class_id="-1"></Parent>
															<Children>
																<count>0</count>
																<item_version>0</item_version>
															</Children>
															<IncomingSynapses>
																<count>0</count>
																<item_version>0</item_version>
															</IncomingSynapses>
														</DendridesRoot>
														<AxonRoot class_id_reference="3" object_id="_14">
															<SpikeEmitter>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>-59.149826402533748</x>
																		<y>0</y>
																		<z>-32.525359321742584</z>
																	</Position>
																	<is_user_movable>0</is_user_movable>
																</SpatialObject>
																<is_spiking>0</is_spiking>
															</SpikeEmitter>
															<SpikeReceiver>
																<SpatialObject>
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
																	</SimulationObject>
																	<Position>
																		<x>-59.149826402533748</x>
																		<y>0</y>
																		<z>-32.525359321742584</z>
																	</Position>
																	<is_user_movable>0</is_user_movable>
																</SpatialObject>
																<spike_received>0</spike_received>
																<IncomingAxons>
																	<count>0</count>
																	<item_version>0</item_version>
																</IncomingAxons>
															</SpikeReceiver>
															<Receivers>
																<count>1</count>
																<item_version>0</item_version>
																<item class_id_reference="2" object_id="_15">
																	<SimulationObject>
																		<m_neuron class_id_reference="0" object_id_reference="_11"></m_neuron>
																	</SimulationObject>
																	<Emitter class_id_reference="3" object_id_reference="_14"></Emitter>
																	<Receiver class_id_reference="4" object_id_reference="_10"></Receiver>
																	<Speed>55</Speed>
																	<ActionPotentials>
																		<count>0</count>
																		<item_version>0</item_version>
																	</ActionPotentials>
																</item>
															</Receivers>
														</AxonRoot>
													</m_neuron>
												</SimulationObject>
												<Position>
													<x>16.742388096198397</x>
													<y>0</y>
													<z>2.6676121113457043</z>
												</Position>
												<is_user_movable>0</is_user_movable>
											</SpatialObject>
											<spike_received>0</spike_received>
											<IncomingAxons>
												<count>1</count>
												<item_version>0</item_version>
												<item class_id_reference="2" object_id_reference="_15"></item>
											</IncomingAxons>
										</SpikeReceiver>
										<PostsynapticDendriticNode class_id_reference="5" object_id_reference="_3"></PostsynapticDendriticNode>
										<Weight>30</Weight>
										<Time_constant>80</Time_constant>
										<ActivePotentials>
											<count>0</count>
											<item_version>0</item_version>
										</ActivePotentials>
									</item>
								</IncomingSynapses>
							</DendridesRoot>
							<AxonRoot class_id_reference="3" object_id="_16">
								<SpikeEmitter>
									<SpatialObject>
										<SimulationObject>
											<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
										</SimulationObject>
										<Position>
											<x>32.742388096198397</x>
											<y>0</y>
											<z>2.6676121113457043</z>
										</Position>
										<is_user_movable>0</is_user_movable>
									</SpatialObject>
									<is_spiking>0</is_spiking>
								</SpikeEmitter>
								<SpikeReceiver>
									<SpatialObject>
										<SimulationObject>
											<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
										</SimulationObject>
										<Position>
											<x>32.742388096198397</x>
											<y>0</y>
											<z>2.6676121113457043</z>
										</Position>
										<is_user_movable>0</is_user_movable>
									</SpatialObject>
									<spike_received>0</spike_received>
									<IncomingAxons>
										<count>0</count>
										<item_version>0</item_version>
									</IncomingAxons>
								</SpikeReceiver>
								<Receivers>
									<count>1</count>
									<item_version>0</item_version>
									<item class_id_reference="2" object_id="_17">
										<SimulationObject>
											<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
										</SimulationObject>
										<Emitter class_id_reference="3" object_id_reference="_16"></Emitter>
										<Receiver class_id_reference="3" object_id_reference="_0"></Receiver>
										<Speed>5</Speed>
										<ActionPotentials>
											<count>0</count>
											<item_version>0</item_version>
										</ActionPotentials>
									</item>
								</Receivers>
							</AxonRoot>
						</m_neuron>
					</SimulationObject>
					<Position>
						<x>123.58917709601513</x>
						<y>0</y>
						<z>7.7306801749853218</z>
					</Position>
					<is_user_movable>1</is_user_movable>
				</SpatialObject>
				<is_spiking>0</is_spiking>
			</SpikeEmitter>
			<SpikeReceiver>
				<SpatialObject>
					<SimulationObject>
						<m_neuron class_id_reference="0" object_id_reference="_1"></m_neuron>
					</SimulationObject>
					<Position>
						<x>123.58917709601513</x>
						<y>0</y>
						<z>7.7306801749853218</z>
					</Position>
					<is_user_movable>1</is_user_movable>
				</SpatialObject>
				<spike_received>0</spike_received>
				<IncomingAxons>
					<count>0</count>
					<item_version>0</item_version>
				</IncomingAxons>
			</SpikeReceiver>
			<Receivers>
				<count>0</count>
				<item_version>0</item_version>
			</Receivers>
		</item>
		<item class_id_reference="3" object_id_reference="_16"></item>
		<item class_id_reference="5" object_id_reference="_13"></item>
		<item class_id_reference="5" object_id_reference="_3"></item>
		<item class_id_reference="2" object_id_reference="_17"></item>
		<item class_id_reference="3" object_id_reference="_8"></item>
		<item class_id_reference="0" object_id_reference="_1"></item>
		<item class_id_reference="3" object_id_reference="_14"></item>
		<item class_id_reference="5" object_id_reference="_7"></item>
		<item class_id_reference="0" object_id_reference="_5"></item>
		<item class_id_reference="0" object_id_reference="_11"></item>
		<item class_id_reference="4" object_id_reference="_4"></item>
		<item class_id_reference="2" object_id_reference="_9"></item>
		<item class_id_reference="4" object_id_reference="_10"></item>
		<item class_id_reference="2" object_id_reference="_15"></item>
		<item</boost_serialization>

