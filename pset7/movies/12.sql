SELECT movies.title AS title FROM movies
JOIN stars, people
ON
    stars.movie_id = movies.id AND
    stars.person_id = people.id
WHERE
    people.name = 'Helena Bonham Carter'
INTERSECT
SELECT movies.title FROM movies
JOIN stars, people
ON
    stars.movie_id = movies.id AND
    stars.person_id = people.id
WHERE
    people.name = 'Johnny Depp';