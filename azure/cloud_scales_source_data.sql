-- -------------------------------------------------------------
-- TablePlus 2.9.1(264)
--
-- https://tableplus.com/
--
-- Database: cloud_scales
-- Generation Time: 2020-05-17 18:40:25.6370
-- -------------------------------------------------------------


-- This script only contains the table creation statements and does not fully represent the table in the database. It's still missing: sequences, indices, triggers. Do not use it as a backup.

CREATE TABLE [dbo].[cloud_scales_source_data] (
    [id] bigint,
    [source] varchar(64),
    [type] varchar(64),
    [name] varchar(64),
    [event] varchar(64),
    [data] decimal(18,0),
    [device_id] varchar(24),
    [published_at] datetime,
    PRIMARY KEY ([id])
);

