SELECT songs.name FROM songs
JOIN artists on artists.id = songs.artist_id
WHERE songs.name LIKE '%feat.%';