if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[BK_SAT_WAREHOUSE]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[BK_SAT_WAREHOUSE]
GO

CREATE TABLE [dbo].[BK_SAT_WAREHOUSE] (
	[strAccountID] [char] (20) COLLATE Korean_Wansung_CI_AS NOT NULL ,
	[nMoney] [int] NOT NULL ,
	[warehouseData] [varchar] (1600) COLLATE Korean_Wansung_CI_AS NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[BK_SAT_WAREHOUSE] WITH NOCHECK ADD 
	CONSTRAINT [PK_BK_SAT_WAREHOUSE] PRIMARY KEY  CLUSTERED 
	(
		[strAccountID]
	)  ON [PRIMARY] 
GO

