import React, { useState } from 'react'
import MapView from './components/MapView'
import { vehicles } from './data/vehicles'
import './App.css'

function App() {
  const [showMap, setShowMap] = useState(false)
  const [selectedVehicle, setSelectedVehicle] = useState(null)
  const [searchTerm, setSearchTerm] = useState('')

  const handleShowMap = (vehicle = null) => {
    setSelectedVehicle(vehicle)
    setShowMap(true)
  }

  const formatDate = (dateString) => {
    const date = new Date(dateString);
    const now = new Date();
    const diffMinutes = Math.floor((now - date) / 60000);
    
    if (diffMinutes < 60) {
      return `${diffMinutes}m ago`;
    } else if (diffMinutes < 1440) {
      return `${Math.floor(diffMinutes / 60)}h ago`;
    } else {
      return date.toLocaleDateString();
    }
  }

  const getStatusColor = (lastUpdated) => {
    const date = new Date(lastUpdated);
    const now = new Date();
    const diffMinutes = Math.floor((now - date) / 60000);
    
    if (diffMinutes < 30) return 'active';
    if (diffMinutes < 120) return 'warning';
    return 'inactive';
  }

  const filteredVehicles = vehicles.filter(vehicle =>
    vehicle.number.toLowerCase().includes(searchTerm.toLowerCase()) ||
    vehicle.supervisor.toLowerCase().includes(searchTerm.toLowerCase())
  );

  return (
    <div className="app">
      <header className="header">
        <div className="header-content">
          <div className="header-title">
            <h1>Electric Vehicle Fleet</h1>
            <p className="subtitle">Real-time vehicle tracking and management</p>
          </div>
          <div className="header-stats">
            <div className="stat-card">
              <div className="stat-value">{vehicles.length}</div>
              <div className="stat-label">Total Vehicles</div>
            </div>
            <div className="stat-card">
              <div className="stat-value">
                {vehicles.filter(v => getStatusColor(v.lastUpdated) === 'active').length}
              </div>
              <div className="stat-label">Active Now</div>
            </div>
          </div>
        </div>
      </header>
      
      <main className="main-content">
        <div className="controls-section">
          <div className="search-bar">
            <span className="search-icon">
              <svg width="20" height="20" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path d="M9 17A8 8 0 1 0 9 1a8 8 0 0 0 0 16zM18 18l-4.35-4.35" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round"/>
              </svg>
            </span>
            <input
              type="text"
              placeholder="Search by vehicle number or supervisor..."
              value={searchTerm}
              onChange={(e) => setSearchTerm(e.target.value)}
              className="search-input"
            />
          </div>
          <button 
            className="show-map-btn"
            onClick={() => handleShowMap()}
          >
            View All on Map
          </button>
        </div>

        {filteredVehicles.length === 0 ? (
          <div className="no-results">
            <p>No vehicles found matching "{searchTerm}"</p>
          </div>
        ) : (
          <div className="vehicle-list">
            {filteredVehicles.map((vehicle) => {
              const status = getStatusColor(vehicle.lastUpdated);
              return (
                <div key={vehicle.id} className="vehicle-card">
                  <div className="card-header">
                    <h3 className="vehicle-number">{vehicle.number}</h3>
                    <span className={`status-badge ${status}`}>
                      <span className="status-dot"></span>
                      {status === 'active' ? 'Active' : status === 'warning' ? 'Idle' : 'Offline'}
                    </span>
                  </div>
                  
                  <div className="card-body">
                    <div className="info-row">
                      <span className="info-icon">
                        <svg width="18" height="18" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                          <path d="M10 10a3 3 0 1 0 0-6 3 3 0 0 0 0 6zM10 12c-4.42 0-8 2.69-8 6v1h16v-1c0-3.31-3.58-6-8-6z" fill="currentColor"/>
                        </svg>
                      </span>
                      <div className="info-content">
                        <span className="info-label">Supervisor</span>
                        <span className="info-value">{vehicle.supervisor}</span>
                      </div>
                    </div>
                    
                    <div className="info-row">
                      <span className="info-icon">
                        <svg width="18" height="18" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                          <path d="M10 2C6.69 2 4 4.69 4 8c0 4.5 6 11 6 11s6-6.5 6-11c0-3.31-2.69-6-6-6zm0 8a2 2 0 1 1 0-4 2 2 0 0 1 0 4z" fill="currentColor"/>
                        </svg>
                      </span>
                      <div className="info-content">
                        <span className="info-label">Location</span>
                        <span className="info-value">
                          {vehicle.location.latitude.toFixed(2)}°, {vehicle.location.longitude.toFixed(2)}°
                        </span>
                      </div>
                    </div>
                    
                    <div className="info-row">
                      <span className="info-icon">
                        <svg width="18" height="18" viewBox="0 0 20 20" fill="none" xmlns="http://www.w3.org/2000/svg">
                          <circle cx="10" cy="10" r="8" stroke="currentColor" strokeWidth="2" fill="none"/>
                          <path d="M10 5v5l3 3" stroke="currentColor" strokeWidth="2" strokeLinecap="round"/>
                        </svg>
                      </span>
                      <div className="info-content">
                        <span className="info-label">Last Update</span>
                        <span className="info-value">{formatDate(vehicle.lastUpdated)}</span>
                      </div>
                    </div>
                  </div>
                  
                  <button 
                    className="map-btn"
                    onClick={() => handleShowMap(vehicle)}
                  >
                    View on Map
                  </button>
                </div>
              );
            })}
          </div>
        )}
      </main>
      
      {showMap && (
        <MapView
          vehicles={vehicles}
          selectedVehicle={selectedVehicle}
          onClose={() => setShowMap(false)}
        />
      )}
    </div>
  )
}

export default App
