DROP TABLE IF EXISTS `todo`;

CREATE TABLE `todo` (
      `id` int(10) NOT NULL AUTO_INCREMENT,
      `todo` varchar(255) NOT NULL,
      `order` int(10) NOT NULL,
      `user_id` int(10) NOT NULL,
      PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=UTF8;

insert into `todo` values (1,'Go Shopping',1, 1), (2,'Take the dog for a walk', 2, 1), (3,'Go swimming', 3, 1), (4,'Go to the Gym', 4, 1);
