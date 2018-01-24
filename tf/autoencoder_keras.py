from __future__ import print_function, division

# Simplest possible autoencoder - 1 hidden layer

import numpy as np
from keras.layers import Input, Dense
from keras.models import Model
from keras.callbacks import TensorBoard


data = np.loadtxt("../build/GSE8157.csv", delimiter=",")
data = data.T

encoding_dim = 500
input_dim = data.shape[1]

input_data = Input(shape=(input_dim,))
encoded = Dense(encoding_dim, activation='relu')(input_data)
decoded = Dense(input_dim, activation='sigmoid')(encoded)
autoencoder = Model(input_data, decoded)

encoder = Model(input_data, encoded)
encoded_input = Input(shape=(encoding_dim,))
decoder_layer = autoencoder.layers[-1]
decoder = Model(encoded_input, decoder_layer(encoded_input))

autoencoder.compile(optimizer='adadelta', loss='binary_crossentropy')

tb = TensorBoard(log_dir='./logs',batch_size=4)

autoencoder.fit(data, data,
                epochs=20,
                batch_size=4,
                shuffle=True,
                validation_data=(data,data),
                callbacks=[tb])
