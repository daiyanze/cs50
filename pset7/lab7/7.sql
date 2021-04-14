SELECT AVG(energy) from songs
LEFT JOIN artists ON songs.artist_id = artists.id
WHERE artists.name = 'Drake';