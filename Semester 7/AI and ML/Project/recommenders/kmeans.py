from dataframes import df_games_metadata_cleaned
import numpy as np
from sklearn.cluster import KMeans

class KMeansRecommender:
    def __init__(self, df_games=df_games_metadata_cleaned):
        self.df_games = df_games.copy()
        self.df_games.set_index("appid", inplace=True, drop=True)
        self.feature_cols = [col for col in self.df_games.columns if self.df_games[col].dtype in [np.float32, np.float64]]

        X = self.df_games[self.feature_cols].values
        self.model = KMeans(n_clusters=20)
        self.df_games["cluster"] = self.model.fit_predict(X)

        self.game_vectors = {}
        for idx, row in self.df_games.iterrows():
            self.game_vectors[idx] = row[self.feature_cols].values

    def recommend(self, user_games, k=5):
        valid = [g for g in user_games if g in self.game_vectors]
        user_vec = np.mean([self.game_vectors[g] for g in valid], axis=0)

        cluster = self.model.predict([user_vec])[0]
        sub = self.df_games[self.df_games["cluster"] == cluster]
        sub_vecs = {idx: self.game_vectors[idx] for idx in sub.index}

        dists = []
        for idx, vec in sub_vecs.items():
            if idx in valid:
                continue
            dist = np.linalg.norm(user_vec - vec)
            dists.append((idx, dist))
            
        dists.sort(key=lambda x: x[1])
        top_k = [x[0] for x in dists[:k]]
        return top_k