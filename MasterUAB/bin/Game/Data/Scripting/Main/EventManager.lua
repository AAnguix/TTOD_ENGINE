class 'CEventManager'
function CEventManager:__init( )
	self.Events = {}
	self.PendingSubscribe = {}
	self.PendingUnsubscribe = {}
end
function CEventManager:FireEvent( eventName, ... )
	self.Events[ eventName ] = self.Events[ eventName ] or {}
	
	for subscriber,_ in pairs( self.Events[ eventName ] ) do
		subscriber[ eventName ]( subscriber, ... )
	end
end
function CEventManager:Update( )
	for eventName, subscriberList in pairs( self.PendingSubscribe ) do
		for subscriber,_ in pairs( subscriberList ) do
			self:Subscribe( subscriber, eventName, true )
		end
	end
	self.PendingSubscribe = {}
	
	for eventName, unsubscriberList in pairs( self.PendingUnsubscribe ) do
		for unsubscriber,_ in pairs( unsubscriberList ) do
			self:Unsubscribe( unsubscriber, eventName, true )
		end
	end
	self.PendingUnsubscribe = {}
end
function CEventManager:Subscribe( subscriber, eventName, bUpdating )
	if ( bUpdating ) then
		self.Events[ eventName ] = self.Events[ eventName ] or {}
		self.Events[ eventName ][ subscriber ] = true
	else
		self.PendingSubscribe[ eventName ] = self.PendingSubscribe[ eventName ] or {}
		self.PendingSubscribe[ eventName ][ subscriber ] = true
	end
end
function CEventManager:Unsubscribe( unsubscriber, eventName, bUpdating )
	if ( bUpdating ) then
		self.Events[ eventName ] = self.Events[ eventName ] or {}
		self.Events[ eventName ][ unsubscriber ] = nil
	else
		self.PendingUnsubscribe[ eventName ] = self.PendingUnsubscribe[ eventName ] or {}
		self.PendingUnsubscribe[ eventName ][ unsubscriber ] = nil
	end
end
function CEventManager:PrintEvents()
	-- for i=1, (#self.Events) do
		-- local l_EventName = self.Events[i]
		-- for subscriber,_ in pairs( self.Events[ l_EventName ] ) do
			-- subscriber[ l_EventName ]( subscriber, ... )
		-- end
	-- end
end