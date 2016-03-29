function InitializeGUI()
	-- g_GUIManager:AddButton("button1","gui1_b1","gui1_b2","gui1_b3")
	g_GUIManager:AddButton("button2","gui2_b1","gui2_b2","gui2_b3")
	-- g_GUIManager:AddButton("button3","gui3_b1","gui3_b2","gui3_b3")
	-- g_GUIManager:AddButton("button4","gui4_b1","gui4_b2","gui4_b3")
	-- g_GUIManager:AddButton("button5","gui5_b1","gui5_b2","gui5_b3")
	-- g_GUIManager:AddButton("button6","gui6_b1","gui6_b2","gui6_b3")
	g_GUIManager:AddSlider("slider1","base","top","handle","pressed_handle")
end

function UpdateGUI()

	local l_Pos = SGUIPosition(350,50,256,64)
	g_GUIManager:DoSlider("7","slider1",l_Pos,1.0,10.0,1.0)
	-- local l_Pos = SGUIPosition(50,50,64,64)
	-- g_GUIManager:DoButton("1","button1",l_Pos)
	
	-- local l_Pos2 = SGUIPosition(250,150,64,64)
	-- g_GUIManager:DoButton("2","button2",l_Pos2)
	
	-- local l_Pos3 = SGUIPosition(450,150,64,64)
	-- g_GUIManager:DoButton("3","button3",l_Pos3)
	
	-- local l_Pos3 = SGUIPosition(250,250,64,64)
	-- g_GUIManager:DoButton("3","button3",l_Pos3)
	
	-- local l_Pos4 = SGUIPosition(350,350,64,64)
	-- g_GUIManager:DoButton("4","button4",l_Pos4)
	
	-- local l_Pos5 = SGUIPosition(450,450,64,64)
	-- g_GUIManager:DoButton("5","button5",l_Pos5)
	
	-- local l_Pos6 = SGUIPosition(550,550,64,64)
	-- g_GUIManager:DoButton("6","button6",l_Pos6)
end