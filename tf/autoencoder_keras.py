from __future__ import print_function, division

# Simplest possible autoencoder - 1 hidden layer

import numpy as np
from keras.layers import Input, Dense
from keras.models import Model
from keras.callbacks import TensorBoard


data = np.loadtxt("../build/GSE8157.csv", delimiter=",")
data = data.T

hidden_dim = 1000
encoding_dim = 500
input_dim = data.shape[1]

input_data = Input(shape=(input_dim,))
h1 = Dense(hidden_dim, activation='relu')(input_data)
encoded = Dense(encoding_dim, activation='relu')(h1)
h2 = Dense(hidden_dim, activation='relu')(encoded)
decoded = Dense(input_dim, activation='sigmoid')(h2)
autoencoder = Model(input_data, decoded)

# encoder = Model(input_data, encoded)
# encoded_input = Input(shape=(encoding_dim,))
# decoder_layer = autoencoder.layers[-1]
# decoder = Model(encoded_input, decoder_layer(encoded_input))

def transcriptwise_rmse()

autoencoder.compile(optimizer='adadelta', loss='binary_crossentropy')

tb = TensorBoard(log_dir='./logs',batch_size=4)

autoencoder.fit(data, data,
                epochs=20,
                batch_size=4,
                shuffle=True,
                validation_data=(data,data),
                callbacks=[tb])

# get reconstructions
data_pred = autoencoder.predict(data)

# compute probe-wise RMSE
rmse = np.sqrt(np.mean(np.square((data_pred - data)), axis=0))
