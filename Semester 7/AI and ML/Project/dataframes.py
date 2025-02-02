import pandas as pd

df_user_data = pd.read_csv(
    'dataset/user_data.csv',
    header=None,
    names=["user_id", "game", "behavior", "value", "dummy"],
)

df_user_data_cleaned = pd.read_csv(
    'dataset/user_data_cleaned.csv',
)

df_games_metadata_cleaned = pd.read_csv(
    'dataset/games_metadata_cleaned.csv',
)

df_names_appid = pd.read_csv(
    'dataset/names_appid_map.csv',
)