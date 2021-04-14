SELECT people.name FROM movies
JOIN stars, people
ON
    stars.movie_id = movies.id AND
    people.id = stars.person_id
WHERE movies.title = 'Toy Story';