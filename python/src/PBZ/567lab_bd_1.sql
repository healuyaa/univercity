/*
SQLyog Community v13.2.0 (64 bit)
MySQL - 8.0.34 : Database - lab567
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`lab567` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `lab567`;

/*Table structure for table `table1` */

DROP TABLE IF EXISTS `table1`;

CREATE TABLE `table1` (
  `code_model` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `model` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `car_model` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `car_manufacturer` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `id` int NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `table1` */

insert  into `table1`(`code_model`,`model`,`car_model`,`car_manufacturer`,`id`) values 
('123GHY77F','submath1','nothing1','FirstT Manufacture',1),
('NVSUU22',' justi',' ellov',' Third',2),
('t ','t ','t ','t ',3),
('ch',' ch',' ch',' ch',4);

/*Table structure for table `table2` */

DROP TABLE IF EXISTS `table2`;

CREATE TABLE `table2` (
  `code_model` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `number_model` int DEFAULT NULL,
  `color` varchar(10) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci DEFAULT NULL,
  `mileage` int DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  `id` int NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `table2` */

insert  into `table2`(`code_model`,`number_model`,`color`,`mileage`,`price`,`id`) values 
('FDS6667',7213781,'white',1,148889.15,1),
('NVSUU22',94828942,'red',1,270132.29,2);

/*Table structure for table `table3` */

DROP TABLE IF EXISTS `table3`;

CREATE TABLE `table3` (
  `number_model` int DEFAULT NULL,
  `year_of_issue` int DEFAULT NULL,
  `engine_capacity` int DEFAULT NULL,
  `id` int NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

/*Data for the table `table3` */

insert  into `table3`(`number_model`,`year_of_issue`,`engine_capacity`,`id`) values 
(99339443,1979,201,1),
(7213781,1994,170,2);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
