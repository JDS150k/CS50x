--In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
--Your query should output a table with a single column for the name of each person.
--There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
--Kevin Bacon himself should not be included in the resulting list.

--PSEUDOCODE: Select people if they are not KB and their movie is in (set of KB movies)

SELECT name from people
JOIN stars ON people.id=stars.person_id
JOIN movies ON stars.movie_id=movies.id
WHERE name != "Kevin Bacon" AND birth != 1958
AND movies.id IN (SELECT movies.id FROM movies JOIN stars ON movies.id=stars.movie_id JOIN people ON stars.person_id=people.id WHERE people.name = "Kevin Bacon" AND people.birth = 1958)
