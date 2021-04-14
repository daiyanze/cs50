SELECT people.name AS 'name' FROM people
JOIN directors, movies, ratings
ON
    directors.person_id = people.id AND
    movies.id = directors.movie_id AND
    ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0;