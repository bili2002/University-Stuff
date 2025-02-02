import numpy as np

from dataframes import df_games_metadata_cleaned
from utils import cosine_similarity, find_games_names, find_games_id

class ContentBasedRecommender:
    def __init__(self, df_games=df_games_metadata_cleaned):
        self.df_games = df_games.copy()
        self.df_games.set_index("appid", inplace=True, drop=True)
        self.feature_cols = [col for col in self.df_games.columns if self.df_games[col].dtype in [np.float32, np.float64]]

        self.game_vectors = {}
        for game_id, row in self.df_games.iterrows():
            self.game_vectors[game_id] = row[self.feature_cols].values

    def recommend(self, user_games, k):
        valid_games = [g for g in user_games if g in self.game_vectors]
        user_embedding = np.mean([self.game_vectors[g] for g in valid_games], axis=0)

        similarities = []
        for game_id, vec in self.game_vectors.items():
            if game_id in valid_games:
                continue
                
            sim_score = cosine_similarity(user_embedding, vec)
            similarities.append((game_id, sim_score))
        
        similarities.sort(key=lambda x: x[1], reverse=True)
        top_k = [x[0] for x in similarities[:k]]
        return top_k
