SELECT movies.title AS 'title', ratings.rating AS 'rating' FROM movies
JOIN ratings
ON ratings.movie_id = movies.id
WHERE movies.year = 2010
ORDER BY rating DESC, title ASC;