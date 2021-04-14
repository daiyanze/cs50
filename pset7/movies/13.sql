SELECT DISTINCT(people.name) FROM movies
JOIN stars, people
ON
    stars.movie_id = movies.id AND
    stars.person_id = people.id
WHERE
    people.name != 'Kevin Bacon' AND
    movies.title IN (
        SELECT movies.title FROM movies
        JOIN stars, people
        ON
            stars.movie_id = movies.id AND
            stars.person_id = people.id
        WHERE
            people.name = 'Kevin Bacon' AND people.birth = 1958
);