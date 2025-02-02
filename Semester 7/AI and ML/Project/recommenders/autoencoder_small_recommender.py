import numpy as np
import pandas as pd
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Dropout
from keras.optimizers import Adam

from dataframes import df_games_metadata_cleaned
from utils import cosine_similarity

class AutoencoderSmall(Model):
    def __init__(self, input_dim, latent_dim):
        super().__init__()
        self.latent_dim = latent_dim
        self.encoder = Sequential([
            Input((input_dim,)),
            Dense(20, activation='relu'),
            Dropout(0.2),
            Dense(self.latent_dim, activation='tanh') 
        ])
        self.decoder = Sequential([
            Input((self.latent_dim,)),
            Dense(20, activation='relu'),
            Dropout(0.2),
            Dense(input_dim, activation='sigmoid'),
        ])

    def call(self, x):  
        encoded = self.encoder(x)
        decoded = self.decoder(encoded)
        return decoded

class AutoencoderSmallRecommender:
    def __init__(self, df=df_games_metadata_cleaned, latent_dim=5, epochs=20, batch_size=100):
        self.df = df.copy()
        self.df.set_index("appid", inplace=True, drop=True)

        self.feature_cols = [c for c in self.df.columns if self.df[c].dtype in [np.float32, np.float64]]
        self.X = self.df[self.feature_cols].values

        input_dim = self.X.shape[1]
        self.model = AutoencoderSmall(input_dim, latent_dim)
        self.model.compile(optimizer=Adam(learning_rate=0.001), loss='cosine_similarity')
        
        self.model.fit(
            self.X, self.X,
            epochs=epochs,
            batch_size=batch_size,
            validation_split=0.2,
            verbose=1
        )

        self.encoded_games = {}
        encoded_vectors = self.model.encoder(self.X).numpy()
        for i, idx in enumerate(self.df.index):
            self.encoded_games[idx] = encoded_vectors[i]

    def recommend(self, user_games, k=5):
        valid = [g for g in user_games if g in self.encoded_games]
        user_vec = np.mean([self.encoded_games[g] for g in valid], axis=0)
        sims = []
        for appid, vec in self.encoded_games.items():
            if appid in valid:
                continue
            score = cosine_similarity(user_vec, vec)
            sims.append((appid, score))

        sims.sort(key=lambda x: x[1], reverse=True)
        top_k = [x[0] for x in sims[:k]]
        return top_k
