from __future__ import print_function, division

# Basic variational autoencoder written in Keras
# see https://github.com/keras-team/keras/blob/master/examples/variational_autoencoder.py

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

from keras.layers import Input, Dense, Lambda, Layer
from keras.models import Model
from keras import backend as K
from keras import metrics
from keras.callbacks import TensorBoard
from tensorflow.python import debug as tf_debug

sess = K.get_session()
sess = tf_debug.LocalCLIDebugWrapperSession(sess)
K.set_session(sess)

data = np.loadtxt("../build/GSE8157.csv", delimiter=",")
data = data.T[:40,:]  # I wish keras could handle 'leftover' training samples

input_dim = data.shape[1]
batch_size = 4
intermediate_dim = 500
latent_dim = 100
n_epochs = 20
epsilon_std = 1.0

x = Input(shape=(input_dim,))  # inputs
h = Dense(intermediate_dim, activation='tanh')(x)  # h_q
z_mean = Dense(latent_dim)(h)  # mu
z_log_var = Dense(latent_dim)(h)  # log_sigma

def sampling(args):
    z_mean, z_log_var = args
    epsilon = K.random_normal(shape=(K.shape(z_mean)[0], latent_dim), mean=0.,
                              stddev=epsilon_std)
    return z_mean + K.exp(z_log_var / 2) * epsilon

z = Lambda(sampling, output_shape=(latent_dim,))([z_mean, z_log_var])

decoder_h = Dense(intermediate_dim, activation='tanh')
decoder_mean = Dense(input_dim, activation='sigmoid')
h_decoded = decoder_h(z)
x_decoded_mean = decoder_mean(h_decoded)


# loss layer
class VariationalLayer(Layer):
    def __init__(self, **kwargs):
        self.is_placeholder = True
        super(VariationalLayer, self).__init__(**kwargs)

    def vae_loss(self, x, x_decoded_mean):
        #xent_loss = input_dim * metrics.binary_crossentropy(x, x_decoded_mean)
        xent_loss = K.sum(K.binary_crossentropy(x, x_decoded_mean), axis=1)
        #kl_loss = -0.5 * K.sum(1 + z_log_var - K.square(z_mean) - K.exp(z_log_var), axis=-1)
        kl_loss = 0.5 * K.sum(K.exp(z_log_var) + K.square(z_mean) - 1. - z_log_var, axis=1)
        #return K.mean(xent_loss + kl_loss)
        return xent_loss + kl_loss

    def call(self, inputs):
        x = inputs[0]
        x_decoded_mean = inputs[1]
        loss = self.vae_loss(x, x_decoded_mean)
        self.add_loss(loss, inputs=inputs)
        return x


y = VariationalLayer()([x, x_decoded_mean])
vae = Model(x, y)  # Build the model, containing an instance of VariationalLayer
vae.compile(optimizer='rmsprop', loss=None)

vae.fit(data,
        epochs=n_epochs,
        batch_size=batch_size,
        shuffle=True,
        validation_data=(data, None))

encoder = Model(x, z_mean)

data_encoded = encoder.predict(data, batch_size=batch_size)
plt.figure(figsize=(6,6))
plt.scatter(data_encoded[:,0], data_encoded[:,1])  # just plot first two encoded dims
plt.show()
