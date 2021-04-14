SELECT DISTINCT(people.name) AS 'name' FROM movies
JOIN stars, people
on
    stars.movie_id = movies.id AND
    stars.person_id = people.id
WHERE movies.year = 2004
ORDER BY people.birth;