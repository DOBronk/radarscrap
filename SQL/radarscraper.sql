SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

CREATE DATABASE IF NOT EXISTS `radarscraper` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
USE `radarscraper`;

DROP TABLE IF EXISTS `locations`;
CREATE TABLE `locations` (
  `id` int NOT NULL,
  `segment_id` int DEFAULT NULL,
  `radar_id` bigint NOT NULL,
  `hm` float NOT NULL,
  `from_lat` float NOT NULL,
  `from_lon` float NOT NULL,
  `to_lat` float NOT NULL,
  `to_lon` float NOT NULL,
  `lat` float NOT NULL,
  `lon` float NOT NULL,
  `hits` int NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='Speed trap lat/lon, 3 value pairs: from, to, actual';

DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `id` int NOT NULL,
  `runtime` float NOT NULL,
  `radar_count` int NOT NULL,
  `road_count` int NOT NULL,
  `created` int NOT NULL,
  `updated` int NOT NULL,
  `exit_code` int NOT NULL,
  `errors` text,
  `started_at` timestamp NOT NULL,
  `finished_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

DROP TABLE IF EXISTS `radars`;
CREATE TABLE `radars` (
  `id` int NOT NULL,
  `radar_id` bigint NOT NULL,
  `segment_id` int NOT NULL,
  `location_id` int DEFAULT NULL,
  `hm` float NOT NULL,
  `start` tinytext CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `end` tinytext CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `reason` tinytext NOT NULL,
  `event` tinytext NOT NULL,
  `begin_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `end_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

DROP TABLE IF EXISTS `roads`;
CREATE TABLE `roads` (
  `id` int NOT NULL,
  `name` varchar(5) NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

DROP TABLE IF EXISTS `segments`;
CREATE TABLE `segments` (
  `id` int NOT NULL,
  `start` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `end` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `road_id` int NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;


ALTER TABLE `locations`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `from_lat` (`from_lat`,`from_lon`,`to_lat`,`to_lon`,`lat`,`lon`),
  ADD KEY `segment_id` (`segment_id`,`radar_id`);

ALTER TABLE `log`
  ADD PRIMARY KEY (`id`),
  ADD KEY `finished_at` (`finished_at`),
  ADD KEY `started_at` (`started_at`);

ALTER TABLE `radars`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `radar_ids` (`radar_id`),
  ADD KEY `segment_id` (`segment_id`);

ALTER TABLE `roads`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`);

ALTER TABLE `segments`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `start` (`start`,`end`),
  ADD KEY `road_id` (`road_id`);


ALTER TABLE `locations`
  MODIFY `id` int NOT NULL AUTO_INCREMENT;

ALTER TABLE `log`
  MODIFY `id` int NOT NULL AUTO_INCREMENT;

ALTER TABLE `radars`
  MODIFY `id` int NOT NULL AUTO_INCREMENT;

ALTER TABLE `roads`
  MODIFY `id` int NOT NULL AUTO_INCREMENT;

ALTER TABLE `segments`
  MODIFY `id` int NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
