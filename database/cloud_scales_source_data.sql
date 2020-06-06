/****** Object:  Table [dbo].[cloud_scales_source_data_2]    Script Date: 6/6/2020 3:23:29 PM ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

/* table */
CREATE TABLE [dbo].[cloud_scales_source_data_2](
	[id] [bigint] IDENTITY(1,1) NOT NULL,
	[source] [varchar](64) NULL,
	[type] [varchar](64) NULL,
	[name] [varchar](64) NULL,
	[event] [varchar](64) NULL,
	[data] [decimal](18, 5) NULL,
	[device_id] [varchar](24) NULL,
	[published_at] [datetime] NULL,
	[TARE] [decimal](18, 0) NULL,
	[SCALE] [decimal](8, 0) NULL,
	[SCALE_KG] [decimal](8, 3) NULL,
PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO

/* indexes */
CREATE NONCLUSTERED INDEX [i_data] ON [dbo].[cloud_scales_source_data_2]
(
	[data] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
GO
CREATE NONCLUSTERED INDEX [i_event] ON [dbo].[cloud_scales_source_data_2]
(
	[event] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
GO
CREATE NONCLUSTERED INDEX [i_published_at] ON [dbo].[cloud_scales_source_data_2]
(
	[published_at] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
GO
CREATE NONCLUSTERED INDEX [i_scale_kg] ON [dbo].[cloud_scales_source_data_2]
(
	[scale_kg] ASC
)WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF) ON [PRIMARY]
GO

/* views */
CREATE VIEW[dbo].[cloud_scales_newest_first] AS
SELECT
	*
FROM
[dbo].[cloud_scales_source_data]
WHERE
[DATA] < 5250000
	AND[data] > -50
ORDER BY
[published_at] DESC OFFSET 0 ROWS
GO

CREATE VIEW [dbo].[cloud_scales_with_average] AS SELECT
	*,
	avg([SCALE_KG]) OVER (ORDER BY [published_at] ROWS BETWEEN 100 PRECEDING AND CURRENT ROW) as AGV_SCALE_KG
	FROM
[dbo].[cloud_scales_newest_first]
GO

/* stored proc */


CREATE PROCEDURE [dbo].[insert_cloud_measurement] 
	(@source varchar(64), 
	@TYPE varchar(64), 
	@name varchar(64), 
	@event varchar(64), 
	@data decimal (18, 5), 
	@device_id varchar(24), 
	@published_at datetime)
AS 

DECLARE @tare decimal(8,0)
DECLARE @scale decimal(8,0)
DECLARE @scale_kg decimal(8,3)

IF (CHARINDEX('/weight/',@event) > 0)
	BEGIN
		SET @tare = (@data - 102517)
		SET @scale = @tare / 25.8739495553721
		SET @scale_kg = ROUND(@scale / 1000, 2)
	END
ELSE
	BEGIN
		SET @tare = null
		SET @scale = null
		SET @scale_kg = null
	END

INSERT INTO cloud_scales_source_data (
		[source],
		[TYPE],
		[name],
		[event],
		[data],
		[device_id],
		[published_at],
		[TARE],
		[SCALE],
		[SCALE_KG])
	VALUES(
		@source, 
		@TYPE, 
		@name,
		@event, 
		@data, 
		@device_id, 
		@published_at,
        @tare,
		@scale,
		@scale_kg
		)
GO