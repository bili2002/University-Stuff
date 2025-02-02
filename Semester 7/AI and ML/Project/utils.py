import numpy as np
import pandas as pd
import dataframes
import json
import re

def cosine_similarity(a: np.ndarray, b: np.ndarray) -> float:
    dot = np.dot(a, b)
    norm_a = np.linalg.norm(a)
    norm_b = np.linalg.norm(b)
    if norm_a == 0 or norm_b == 0:
        return 0.0
    return dot / (norm_a * norm_b)

def find_game_name(appid, df=dataframes.df_names_appid):
    row = df.loc[df["appid"] == appid]
    if not row.empty:
        return row["name"].values[0]
    return None

def find_games_names(appids, df=dataframes.df_names_appid):
    return [find_game_name(appid, df) for appid in appids]

def find_game_id(name, df=dataframes.df_names_appid):
    name = clean_game_name(name)
    row = df.loc[df["name"] == name]
    if not row.empty:
        return row["appid"].values[0]
    return None

def find_games_id(names, df=dataframes.df_names_appid):
    return [find_game_id(name, df) for name in names]

EMOJI_PATTERN = re.compile("["
    u"\U0001F600-\U0001F64F"  # emoticons
    u"\U0001F300-\U0001F5FF"  # symbols & pictographs
    u"\U0001F680-\U0001F6FF"  # transport & map symbols
    u"\U0001F1E0-\U0001F1FF"  # flags (iOS)
    "]+", flags=re.UNICODE)

def clean_game_name(name):
    name = name.lower()
    name = EMOJI_PATTERN.sub(r'', name)
    name = re.sub(r'[^a-z0-9 ]', '', name)
    name = re.sub(r'\s+', ' ', name).strip()
    return name
